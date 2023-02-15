#include "elevationchart.hpp"

#include <QDebug>
#include <cmath>
#include "Elevation/elevation.h"
#include <QMetaType>

ElevationChart::ElevationChart(QObject *parent)
    : QObject{parent}
{
    heightmapParser = new Elevation::Elevation(this);
    routeParser = new Elevation::ElevationTools(this);
    qRegisterMetaType<QVector<Elevation::Point>>("QVector<Point>");
    connect(routeParser, &Elevation::ElevationTools::progressTestRouteIntersectGround, this, &ElevationChart::intersectCalculationFinished);
}

void ElevationChart::changeFlightPointAltitude(int index, qreal delta)
{
    QGeoCoordinate coord = m_geopath.coordinateAt(index);
    if(abs(delta) > 2.5)
        coord.setAltitude(coord.altitude() + delta * (0.15));
    if(coord.altitude() <= 0)
        coord.setAltitude(0);
    m_geopath.replaceCoordinate(index, coord);
    setGeopath(m_geopath);
}

void ElevationChart::update(bool vectorChanged)
{
    // ███  build profile if necessary + reset iterators   ███
    if(vectorChanged || points.isEmpty())
        points = heightmapParser->buildGroundProfileForChart(geopath());
    iterator.simple = 0;
    iterator.range = 0;
    iterator.rangeSet = false;

    // ███  axes calculation  ███
    axes.x.max = points.last().x();
    axes.y.max = 0;
    axes.y.roundmax = 0;
    for(QPointF point : points)
    {
        if(axes.y.max < point.y())
            axes.y.max = point.y();
    }
    if(m_logging) qDebug() << "<qplot> profile max elevation:" << axes.y.max << "m";
    for(QGeoCoordinate coord : m_geopath.path())
    {
        if(coord.altitude() > axes.y.max)
        {
            axes.y.max = coord.altitude();
            if(m_logging) qInfo() << "<qplot> flightpath max elevation" << axes.y.max << "m";
        }
    }

    axes.x.power = axes.x.max > 0 ? (int) log10 ((float) axes.x.max) : 1;
    axes.y.power = axes.y.max > 0 ? (int) log10 ((float) axes.y.max) : 1;
    while(axes.x.roundmax < axes.x.max)
    {
        axes.x.roundmax += pow(10, axes.x.power);
    }
    while(axes.y.roundmax < axes.y.max)
    {
        axes.y.roundmax += pow(10, axes.y.power);
    }
    setRealWidth(axes.x.max);
    setRealHeight(axes.y.max);
    setScaleValueX(pow(10, axes.x.power));
    setScaleValueY(pow(10, axes.y.power));
    setScaleCountX((float)axes.x.max / (float)axes.x.scalevalue);
    setScaleCountY((float)axes.y.max * axes.stretch / (float)axes.y.scalevalue);
    setScaleStepX((axes.x.pixelsize) / axes.x.scalecount);
    setScaleStepY((axes.y.pixelsize)/ axes.y.scalecount);


    // ███  flight path & variometer values ███
    QList<QPointF> data;
    QList<bool> errorData;
    QList<float> errorDeltas;
    qreal previous_distance = 0;
    errorData.append(false);
    errorDeltas.append(0);
    for(size_t i = 0; i < m_geopath.path().length(); i++)
    {
        QPointF point;
        qreal deltaS = 0;
        point.setY(m_geopath.path()[i].altitude() * axes.y.pixelsize / (axes.y.max * axes.stretch));
        if(i > 0)
            deltaS = m_geopath.path()[i].distanceTo(m_geopath.path()[i-1]);
        previous_distance += deltaS;
        point.setX(previous_distance * axes.x.pixelsize / axes.x.max);
        data.append(point);

        // ███   variometer error list   ███
        if(i > 0)
        {
            qreal deltaH = m_geopath.path()[i].altitude() - m_geopath.path()[i-1].altitude();
            qreal deltaHmin = variometer.RoD * deltaS / variometer.hV;
            qreal deltaHmax = variometer.RoC * deltaS / variometer.hV;
            if(deltaH > 0 && deltaH > deltaHmax)
            {
                errorData.append(true);
                errorDeltas.append((m_geopath.path()[i-1].altitude() + deltaHmax) * axes.y.pixelsize / (axes.y.max * axes.stretch));
            }
            else if(deltaH < 0 && abs(deltaH) > deltaHmin)
            {
                errorData.append(true);
                errorDeltas.append((m_geopath.path()[i-1].altitude() - deltaHmin) * axes.y.pixelsize / (axes.y.max * axes.stretch));
            }
            else {
                errorData.append(false);
                errorDeltas.append(0);
            }
        }
    }
    setPathData(data);
    setPathErrorList(errorData);
    setPathErrorValueList(errorDeltas);

    // ███   intersections calculation  ███
    routeParser->testRouteIntersectGround(geopath());

    // ███   logging values for debug and requesting canvas update   ███
    emit requestRedraw();
    if(m_logging)
    {
        qInfo()  << "<qplot> calculating values <><> ";
        qDebug() << "<qplot> profile list size: " << points.length() << "pts";
        qDebug() << "<qplot> pixelsize x: " << axes.x.pixelsize << "px";
        qDebug() << "<qplot> pixelsize y: " << axes.y.pixelsize << "px";
        qDebug() << "<qplot> max value x: " << axes.x.max << "m";
        qDebug() << "<qplot> max value y: " << axes.y.max << "m";
        qDebug() << "<qplot> roundmax value x: " << axes.x.roundmax << "m";
        qDebug() << "<qplot> roundmax value y: " << axes.y.roundmax << "m";
        qDebug() << "<qplot> zoom level x: " << zoomX() + 1;
    }
}

void ElevationChart::intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point> &resultPath)
{
    QList<QPointF> _intersectList;
    for(size_t i = 0; i < resultPath.length(); i++)
    {
        if(resultPath[i].isBase())
            continue;
        QPointF _point(resultPath[i].distance() * axes.x.pixelsize / axes.x.max,
                       resultPath[i].altitude() * axes.y.pixelsize / (axes.y.max * axes.stretch));
        _intersectList.append(_point);
    }
    setIntersectList(_intersectList);
    emit requestRedraw();
}

QPointF ElevationChart::iterateSimple(void)
{
    iterator.simple += 1;
    if(iterator.simple < points.length())
    {
        QPointF ret = points.at(iterator.simple);
        ret.setX(ret.x() * pixelWidth() / axes.x.max);
        ret.setY(ret.y() * pixelHeight() / axes.y.max);
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
            int lval1 = rangeStart * axes.x.max;
            int lval2 = rangeStop * axes.x.max;
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
        return QPointF(points[iterator.range].x() * axes.x.pixelsize / axes.x.max,
                       points[iterator.range].y() * axes.y.pixelsize / (axes.y.max * axes.stretch));
    } else {
        iterator.range = 0;
        iterator.rangeSet = false;
        return QPointF(-1, -1);
    }
}
