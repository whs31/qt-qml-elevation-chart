#include "elevationchart.hpp"

#include <QDebug>
#include <cmath>
#include "Elevation/elevation.h"
#include <QMetaType>

ElevationChart::ElevationChart(QObject* parent)
    : QObject{parent}
{
    // submodule types
    heightmapParser = new Elevation::Elevation(this);
    routeParser = new Elevation::ElevationTools(this);

    // qplot private implementations
    fpsCounterImpl = new FPSCounter();

    qRegisterMetaType<QVector<Elevation::Point>>("QVector<Point>");
    connect(routeParser, &Elevation::ElevationTools::progressTestRouteIntersectGround, this, &ElevationChart::intersectCalculationFinished);
}

void ElevationChart::changeFlightPointAltitude(int index, qreal delta)
{
    QGeoCoordinate coord = m_geopath.coordinateAt(index);
    coord.setAltitude(coord.altitude() + delta * (0.15));
    if(coord.altitude() <= 0)
        coord.setAltitude(0);
    m_geopath.replaceCoordinate(index, coord);
    geopathSet(m_geopath);
}

void ElevationChart::updateProfile(void)
{
    update(true);
}

void ElevationChart::update(bool vectorChanged)
{
    // ███  build profile if necessary + reset iterators ███
    if(vectorChanged || points.isEmpty())
        points = heightmapParser->buildGroundProfileForChart(geopath());
    iterator.simple = 0;
    iterator.range = 0;
    iterator.rangeSet = false;

    // ███  axes calculation  ███
    m_realWidth = points.last().x();
    m_realHeight = 0;
    m_roundmaxY = 0;
    for(QPointF point : points)
    {
        if(m_realHeight < point.y())
            m_realHeight = point.y();
    }
    if(m_logging) qDebug() << "<qplot> profile max elevation:" << m_realHeight << "m";
    for(QGeoCoordinate coord : m_geopath.path())
    {
        if(coord.altitude() > m_realHeight)
        {
            m_realHeight = coord.altitude();
            if(m_logging) qInfo() << "<qplot> flightpath max elevation" << m_realHeight << "m";
        }
    }

    m_powerX = m_realWidth > 0 ? (int) log10 ((float) m_realWidth) : 1;
    m_powerY = m_realHeight > 0 ? (int) log10 ((float) m_realHeight) : 1;
    while(m_roundmaxX < m_realWidth)
    {
        m_roundmaxX += pow(10, m_powerX);
    }
    while(m_roundmaxY < m_realHeight)
    {
        m_roundmaxY += pow(10, m_powerY);
    }
    realWidthSet(m_realWidth);
    realHeightSet(m_realHeight);
    scaleValueXSet(pow(10, m_powerX));
    scaleValueYSet(pow(10, m_powerY));
    scaleCountXSet((float)realWidth() / (float)scaleValueX());
    scaleCountYSet((float)realHeight() * verticalStretch() / (float)scaleValueY());
    scaleStepXSet((pixelWidth()) / scaleCountX());
    scaleStepYSet((pixelHeight())/ scaleCountY());


    // ███  flight path & variometer values ███
    QList<QPointF> data;
    QList<QPointF> fixedData;
    QList<bool> errorData;
    QList<float> errorDeltas;
    qreal previous_distance = 0;
    errorData.append(false);
    errorDeltas.append(pixelHeight());
    for(size_t i = 0; i < m_geopath.path().length(); i++)
    {
        QPointF point;
        qreal deltaS = 0;
        point.setY(pixelHeight() - m_geopath.path()[i].altitude() * pixelHeight() / (realHeight() * verticalStretch()));
        if(i > 0)
            deltaS = m_geopath.path()[i].distanceTo(m_geopath.path()[i-1]);
        previous_distance += deltaS;
        point.setX(previous_distance * pixelWidth() / realWidth());
        data.append(point);

        // ███   variometer error list   ███
        if(i > 0)
        {
            qreal deltaH = m_geopath.path()[i].altitude() - m_geopath.path()[i-1].altitude();
            qreal deltaHmin = variometerROD() * deltaS / variometerHV();
            qreal deltaHmax = variometerROC() * deltaS / variometerHV();
            if(deltaH > 0 && deltaH > deltaHmax)
            {
                errorData.append(true);
                errorDeltas.append(pixelHeight() - (m_geopath.path()[i-1].altitude() + deltaHmax) * pixelHeight() / (realHeight() * verticalStretch()));
            }
            else if(deltaH < 0 && abs(deltaH) > deltaHmin)
            {
                errorData.append(true);
                errorDeltas.append(pixelHeight() - (m_geopath.path()[i-1].altitude() - deltaHmin) * pixelHeight() / (realHeight() * verticalStretch()));
            }
            else {
                errorData.append(false);
                errorDeltas.append(pixelHeight());
            }
        }
    }
    pathDataSet(data);
    pathErrorListSet(errorData);
    pathErrorValueListSet(errorDeltas);

    // ███   intersections calculation  ███
    routeParser->testRouteIntersectGround(geopath());

    // ███   logging values for debug and requesting canvas update   ███
    if(vectorChanged || points.isEmpty())
        emit requestRedraw();
    else
        emit requestRedrawPath();
    if(m_logging)
    {
        qInfo()  << "<qplot> calculating values <><> ";
        qDebug() << "<qplot> profile list size: " << points.length() << "pts";
        qDebug() << "<qplot> pixelsize x: " << pixelWidth() << "px";
        qDebug() << "<qplot> pixelsize y: " << pixelHeight() << "px";
        qDebug() << "<qplot> max value x: " << realWidth() << "m";
        qDebug() << "<qplot> max value y: " << realHeight() << "m";
        qDebug() << "<qplot> roundmax value x: " << m_roundmaxX << "m";
        qDebug() << "<qplot> roundmax value y: " << m_roundmaxY << "m";
    }
}

void ElevationChart::intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point>& resultPath)
{
    QList<QPointF> _intersectList;
    for(size_t i = 0; i < resultPath.length(); i++)
    {
        if(resultPath[i].isBase())
            continue;
        QPointF _point(resultPath[i].distance() * pixelWidth() / realWidth(),
                       pixelHeight() - resultPath[i].altitude() * pixelHeight() / (realHeight() * verticalStretch()));
        _intersectList.append(_point);
    }
    intersectListSet(_intersectList);
    emit requestRedrawIntersects();
}

QPointF ElevationChart::iterateSimple(void)
{
    iterator.simple += 1;
    if(iterator.simple < points.length())
    {
        QPointF ret = points.at(iterator.simple);
        ret.setX(ret.x() * pixelWidth() / realWidth());
        ret.setY(ret.y() * pixelHeight() / realHeight());
        return ret;
    } else {
        iterator.simple = 0;
        if(m_logging) qInfo() << "<qplot> redraw finished";
        return QPointF(-1, -1);
    }
}

QPointF ElevationChart::iterateOverRange(float rangeStart, float rangeStop)
{
    if(!iterator.rangeSet)
    {
        iterator.rangeMin = -1;
        iterator.rangeMax = -1;
        iterator.range = 0;
        if(rangeStart == 0)
            iterator.rangeMin = 0;
        if(rangeStop == 1)
            iterator.rangeMax = points.length() - 1;
        if(rangeStart != 0 && rangeStop != 1)
        {
            int lval1 = rangeStart * realWidth();
            int lval2 = rangeStop * realWidth();
            bool _rangeMinFlag = false;
            for(int i = 0; i < points.length(); i++)
            {
                int rval = points[i].x();
                if(lval1 < rval && !_rangeMinFlag)
                {
                    iterator.rangeMin = i > 0 ? i - 1 : 0;
                    _rangeMinFlag = true;
                }
                if(lval2 >= rval)
                    iterator.rangeMax = i < points.length() - 1 ? i + 1 : i;
            }
        }
        Q_ASSERT_X(iterator.rangeMax != -1, "<qplot> range assignment", "upper range not found in vector");
        Q_ASSERT_X(iterator.rangeMax < points.length(), "<qplot> bounds", "rangeMax out of bounds");
        Q_ASSERT_X(iterator.rangeMin >= 0, "<qplot> bounds", "rangeMin lower than 0");
        iterator.rangeSet = true;
        iterator.range = iterator.rangeMin;
    }
    iterator.range++;
    if(iterator.range < iterator.rangeMax)
    {
        return QPointF(points[iterator.range].x() * pixelWidth() / realWidth(),
                points[iterator.range].y() * pixelHeight() / (realHeight() * verticalStretch()));
    } else {
        iterator.range = 0;
        iterator.rangeSet = false;
        return QPointF(-1, -1);
    }
}
