#include "elevationchart.hpp"

#include <QDebug>
#include <cmath>
#include "Elevation/elevation.h"
//#include "RouteTools/elevationtools.h"
ElevationChart::ElevationChart(QObject *parent)
    : QObject{parent}
{
    heightmapParser = new Elevation::Elevation(this);
    //routeParser = new Elevation::ElevationTools(this);
}

void ElevationChart::changeFlightPointAltitude(int index, qreal delta)
{
    QGeoCoordinate coord = m_geopath.coordinateAt(index);
    if(abs(delta) > 2.5)
        coord.setAltitude(coord.altitude() + delta * (0.1 + coord.altitude() / (axes.y.max * 40)));
    if(coord.altitude() <= 0)
        coord.setAltitude(0);
    m_geopath.replaceCoordinate(index, coord);
    setGeopath(m_geopath);
}

void ElevationChart::update(bool vectorChanged)
{
    if(vectorChanged || points.isEmpty())
        points = heightmapParser->buildGroundProfileForChart(geopath());
    iterator.simple = 0;
    iterator.range = 0;
    iterator.rangeSet = false;

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

        // error list
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
    emit requestRedraw();

    if(m_logging)
    {
        qInfo() << "<qplot> calculating values <><> ";
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

//void ElevationChart::intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point> &resultPath)
//{

//}

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

QGeoPath ElevationChart::geopath() const { return m_geopath; }
void ElevationChart::setGeopath(const QGeoPath &path)
{
    m_geopath = path;
    if(m_logging) qInfo() << "<qplot> got new path: " << m_geopath.path();
    emit geopathChanged();

    if(axes.x.pixelsize > 0) update(true);
}
QList<QPointF> ElevationChart::pathData() const { return m_pathData; }
void ElevationChart::setPathData(QList<QPointF> data)
{ if(m_logging) qDebug() << "<qplot> uav path: " << m_pathData;
  m_pathData = data; emit pathDataChanged(); }
QList<bool> ElevationChart::pathErrorList() const { return m_pathErrorList; }
void ElevationChart::setPathErrorList(const QList<bool> &newPathErrorList)
{ if (m_pathErrorList == newPathErrorList) return; m_pathErrorList = newPathErrorList; emit pathErrorListChanged(); }
QList<float> ElevationChart::pathErrorValueList() const { return m_pathErrorValueList; }
void ElevationChart::setPathErrorValueList(const QList<float> &newPathErrorList)
{ if (m_pathErrorValueList == newPathErrorList) return; m_pathErrorValueList = newPathErrorList; emit pathErrorValueListChanged(); }

bool ElevationChart::logging() const { return m_logging; }
void ElevationChart::setLogging(bool state)
{ if (m_logging == state) return; m_logging = state; emit loggingChanged(); }

qreal ElevationChart::pixelWidth() const { return axes.x.pixelsize; }
void ElevationChart::setPixelWidth(qreal value)
{ if (qFuzzyCompare(axes.x.pixelsize, value)) return; axes.x.pixelsize = value; emit pixelWidthChanged();
  if(m_logging) qDebug() << "<qplot> pixelwidth changed <>"; if(!m_geopath.isEmpty() && axes.y.pixelsize > 0) update(false); }
qreal ElevationChart::pixelHeight() const { return axes.y.pixelsize; }
void ElevationChart::setPixelHeight(qreal value)
{ if (qFuzzyCompare(axes.y.pixelsize, value)) return; axes.y.pixelsize = value; emit pixelHeightChanged();
  if(m_logging) qDebug() << "<qplot> pixelheight changed <>"; if(!m_geopath.isEmpty() && axes.x.pixelsize > 0) update(false); }

qreal ElevationChart::offset() const { return axes.offset; }
void ElevationChart::setOffset(qreal value)
{ if (qFuzzyCompare(axes.offset, value)) return; axes.offset = value; emit offsetChanged(); }
qreal ElevationChart::verticalStretch() const { return axes.stretch; }
void ElevationChart::setVerticalStretch(qreal value)
{ if (qFuzzyCompare(axes.stretch, value)) return; axes.stretch = value; emit verticalStretchChanged(); }
qreal ElevationChart::zoomX() const { return axes.x.zoom; }
void ElevationChart::setZoomX(qreal value)
{ if (qFuzzyCompare(axes.x.zoom, value)) return; axes.x.zoom = value; emit zoomXChanged(); }

qreal ElevationChart::realHeight() const { return axes.y.max; }
void ElevationChart::setRealHeight(qreal value) { axes.y.max = value; emit realHeightChanged(); }
qreal ElevationChart::realWidth() const { return axes.x.max; }
void ElevationChart::setRealWidth(qreal value) { axes.x.max = value; emit realWidthChanged(); }

int ElevationChart::scaleValueX() const { return axes.x.scalevalue; }
void ElevationChart::setScaleValueX(int value)
{ if (axes.x.scalevalue == value) return; axes.x.scalevalue = value; emit scaleValueXChanged(); }
int ElevationChart::scaleValueY() const { return axes.y.scalevalue; }
void ElevationChart::setScaleValueY(int value)
{ if (axes.y.scalevalue == value) return; axes.y.scalevalue = value; emit scaleValueYChanged(); }
qreal ElevationChart::scaleCountX() const { return axes.x.scalecount; }
void ElevationChart::setScaleCountX(qreal count)
{ if (axes.x.scalecount == count) return; axes.x.scalecount = count; emit scaleCountXChanged(); }
qreal ElevationChart::scaleCountY() const { return axes.y.scalecount; }
void ElevationChart::setScaleCountY(qreal count)
{ if (axes.y.scalecount == count) return; axes.y.scalecount = count; emit scaleCountYChanged(); }
qreal ElevationChart::scaleStepX() const { return axes.x.scalepixelwidth; }
void ElevationChart::setScaleStepX(qreal value)
{ if (qFuzzyCompare(axes.x.scalepixelwidth, value)) return; axes.x.scalepixelwidth = value; emit scaleStepXChanged(); }
qreal ElevationChart::scaleStepY() const { return axes.y.scalepixelwidth; }
void ElevationChart::setScaleStepY(qreal value)
{ if (qFuzzyCompare(axes.y.scalepixelwidth, value)) return; axes.y.scalepixelwidth = value; emit scaleStepYChanged(); }

qreal ElevationChart::variometerHV() const { return variometer.hV; }
void ElevationChart::setVariometerHV(qreal value)
{ if (qFuzzyCompare(variometer.hV, value)) return; variometer.hV = value; emit variometerHVChanged(); update(false); }
qreal ElevationChart::variometerROC() const { return variometer.RoC; }
void ElevationChart::setVariometerROC(qreal value)
{ if (qFuzzyCompare(variometer.RoC, value)) return; variometer.RoC = value; emit variometerROCChanged(); update(false); }
qreal ElevationChart::variometerROD() const { return variometer.RoD; }
void ElevationChart::setVariometerROD(qreal value)
{ if (qFuzzyCompare(variometer.RoD, value)) return; variometer.RoD = value; emit variometerRODChanged(); update(false); }
