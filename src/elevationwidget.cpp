#include "elevationwidget.hpp"
#include "elevationwidget_p.hpp"

#include <qqml.h>
#include <cmath>
#include "Elevation/elevation.h"
#include <QDebug>
#include <QMetaType>


ElevationWidget::ElevationWidget(QObject *parent)
    : QObject{parent}
    , d_ptr(new ElevationWidgetPrivate(this))
{
    qmlRegisterSingletonInstance("ElevationWidgetImpl", 1, 0, "Impl", d_ptr);
}

QGeoPath ElevationWidget::getGeopath()
{
    Q_D(ElevationWidget);
    return d->geopath;
}

void ElevationWidget::setGeopath(QGeoPath path)
{
    Q_D(ElevationWidget);
    if(path == d->geopath)
        return;
    d->geopath = path;
    d->recalculateWithGeopathChanged();
}

void ElevationWidget::showIndexes(bool state)
{
    Q_D(ElevationWidget);
    d->setShowIndex(state);
}

void ElevationWidget::setVelocity(float velocity)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.velocity = velocity;
    // recalculate (variometer) ()
}

void ElevationWidget::setClimbRate(float rate)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.climbRate = rate;
    // recalculate (variometer) ()
}

void ElevationWidget::setDescendRate(float rate)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.descendRate = rate;
    // recalculate (variometer) ()
}

void ElevationWidget::setPallete(QString backgroundColor, QString foregroundColor, QString chartColor,
                                 QString successColor, QString warningColor, QString errorColor)
{
    Q_D(ElevationWidget);
    d->setColors({ backgroundColor, foregroundColor, chartColor, successColor, warningColor, errorColor });

}

#if(0)
//─────────────────────────────█████████████████████████████████ 🔒 PRIVATE IMPLEMENTATION 🔒 █████████████████████████████████████──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
#endif

ElevationWidgetPrivate::ElevationWidgetPrivate(QObject* parent)
    : QObject{parent}
{
    // submodule types
    heightmapParser = new Elevation::Elevation(this);
    routeParser = new Elevation::ElevationTools(this);

    qRegisterMetaType<QVector<Elevation::Point>>("QVector<Point>");
    connect(routeParser, &Elevation::ElevationTools::progressTestRouteIntersectGround,
            this, &ElevationWidgetPrivate::intersectCalculationFinished);

    recalculateWithGeopathChanged();
}

// █ calculate axes and scales
void ElevationWidgetPrivate::recalculate(bool emitFlag)
{
    if(profile().isEmpty())
        return;

    iterator.range = 0;
    iterator.rangeSet = false;

    axis.x.maxValue = profile().last().x();
    axis.x.roundMaxValue = 0;
    axis.y.maxValue = 0;
    axis.y.roundMaxValue = 0;

    // double check real quick)
    for(QPointF point : profile())
    {
        if(axis.y.maxValue < point.y())
            axis.y.maxValue = point.y();
    }
    for(QGeoCoordinate coord : geopath.path())
    {
        if(coord.altitude() > axis.y.maxValue)
            axis.y.maxValue = coord.altitude();
    }

    int _power_x = axis.x.maxValue > 0 ? (int) log10 ((float) axis.x.maxValue) : 1;
    int _power_y = axis.y.maxValue > 0 ? (int) log10 ((float) axis.y.maxValue) : 1;
    while(axis.x.roundMaxValue < axis.x.maxValue)
    {
        axis.x.roundMaxValue += pow(10, _power_x);
    }
    while(axis.y.roundMaxValue < axis.y.maxValue)
    {
        axis.y.roundMaxValue += pow(10, _power_y);
    }
    axis.x.scaleValue = pow(10, _power_x);
    axis.y.scaleValue = pow(10, _power_y);
    axis.x.scaleCount = (float)axis.x.maxValue / (float)axis.x.scaleValue;
    axis.y.scaleCount = (float)axis.y.maxValue * axis.stretch / (float)axis.y.scaleValue;
    axis.x.scalePixelSize = layout.width / axis.x.scaleCount;
    axis.y.scalePixelSize = layout.height / axis.y.scaleCount;
    setKeyValues({  axis.stretch,                                       // 0
                    axis.x.maxValue, axis.y.maxValue,                   // 1-2
                    axis.x.roundMaxValue, axis.y.roundMaxValue,         // 3-4
                    axis.x.scaleValue, axis.y.scaleValue,               // 5-6
                    axis.x.scaleCount, axis.y.scaleCount,               // 7-8
                    axis.x.scalePixelSize, axis.y.scalePixelSize  });   // 9-10

    calculatePath();

    routeParser->testRouteIntersectGround(geopath);

    if(emitFlag || profile().isEmpty())
        emit requestAll();
    else
        emit requestPath();
}


// █ build profile
void ElevationWidgetPrivate::recalculateWithGeopathChanged()
{
    setProfile(heightmapParser->buildGroundProfileForChart(geopath));
    recalculate(true);
}

void ElevationWidgetPrivate::calculatePath()
{
    QList<QPointF> data;
    qreal previous_distance = 0;
    for(size_t i = 0; i < geopath.path().length(); i++)
    {
        QPointF point;
        qreal deltaS = 0;
        point.setY(layout.height - geopath.path()[i].altitude() * layout.height / (axis.y.maxValue * axis.stretch));
        if(i > 0)
            deltaS = geopath.path()[i].distanceTo(geopath.path()[i-1]);
        previous_distance += deltaS;
        point.setX(previous_distance * layout.width / axis.x.maxValue);
        data.append(point);
    }
    setPath(data);
}

void ElevationWidgetPrivate::intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point>& resultPath)
{
    QList<QPointF> _intersectList;
    for(size_t i = 0; i < resultPath.length(); i++)
    {
        if(resultPath[i].isBase())
            continue;
        QPointF _point(resultPath[i].distance() * layout.width / axis.x.maxValue,
                       layout.height - resultPath[i].altitude() * layout.height / (axis.y.maxValue * axis.stretch));
        _intersectList.append(_point);
    }
    setIntersections(_intersectList);
    emit requestIntersects();
}

void ElevationWidgetPrivate::resize(float w, float h, float zoom_w, float zoom_h)
{
    if(w == layout.width && h == layout.height &&
       zoom_w == layout.horizontal_zoom && zoom_h == layout.vertical_zoom)
        return;
    layout.width = w;
    layout.height = h;
    layout.horizontal_zoom = zoom_w;
    layout.vertical_zoom = zoom_h;
    if(!geopath.isEmpty() && layout.width * layout.height > 0)
        recalculate();
}

QPointF ElevationWidgetPrivate::iterateOverRange(float rangeStart, float rangeStop)
{
    if(!iterator.rangeSet)
    {
        iterator.rangeMin = -1;
        iterator.rangeMax = -1;
        iterator.range = 0;
        if(rangeStart == 0)
            iterator.rangeMin = 0;
        if(rangeStop == 1)
            iterator.rangeMax = m_profile.length() - 1;
        if(rangeStart != 0 && rangeStop != 1)
        {
            int lval1 = rangeStart * axis.x.maxValue;
            int lval2 = rangeStop * axis.x.maxValue;
            bool _rangeMinFlag = false;
            for(int i = 0; i < m_profile.length(); i++)
            {
                int rval = m_profile[i].x();
                if(lval1 < rval && !_rangeMinFlag)
                {
                    iterator.rangeMin = i > 0 ? i - 1 : 0;
                    _rangeMinFlag = true;
                }
                if(lval2 >= rval)
                    iterator.rangeMax = i < m_profile.length() - 1 ? i + 1 : i;
            }
        }
        Q_ASSERT_X(iterator.rangeMax != -1, "<qplot> range assignment", "upper range not found in vector");
        Q_ASSERT_X(iterator.rangeMax < m_profile.length(), "<qplot> bounds", "max range is out of bounds");
        Q_ASSERT_X(iterator.rangeMin >= 0, "<qplot> bounds", "min range is lower than 0");
        iterator.rangeSet = true;
        iterator.range = iterator.rangeMin;
    }
    iterator.range++;
    if(iterator.range < iterator.rangeMax)
    {
        return QPointF(m_profile[iterator.range].x() * layout.width / axis.x.maxValue,
                m_profile[iterator.range].y() * layout.height / (axis.y.maxValue * axis.stretch));
    } else {
        iterator.range = 0;
        iterator.rangeSet = false;
        return QPointF(-1, -1);
    }
}

void ElevationWidgetPrivate::changeFlightPointAltitude(int index, qreal delta)
{
    QGeoCoordinate coord = geopath.coordinateAt(index);
    coord.setAltitude(coord.altitude() + delta * (0.15));
    if(coord.altitude() <= 0)
        coord.setAltitude(0);
    geopath.replaceCoordinate(index, coord);
    recalculateWithGeopathChanged();

    //@BUG: сегфолт_)))))
    //Q_Q(ElevationWidget);
    //emit(q->geopathChanged());
}

QList<QString> ElevationWidgetPrivate::colors() const { return m_colors; }
void ElevationWidgetPrivate::setColors(const QList<QString>& list) {
    if (m_colors == list) return;
    m_colors = list;
    emit colorsChanged();
}

QVector<QPointF> ElevationWidgetPrivate::profile() const { return m_profile; }
void ElevationWidgetPrivate::setProfile(const QVector<QPointF>& vec) {
    if (m_profile == vec) return;
    m_profile = vec;
    emit profileChanged();
    recalculateWithGeopathChanged();
}

QList<QPointF> ElevationWidgetPrivate::path() const { return m_path; }
void ElevationWidgetPrivate::setPath(const QList<QPointF>& list) {
    if (m_path == list) return;
    m_path = list;
    emit pathChanged();
}

QList<float> ElevationWidgetPrivate::keyValues() const { return m_keyValues; }
void ElevationWidgetPrivate::setKeyValues(const QList<float>& values) {
    if (m_keyValues == values) return;
    m_keyValues = values;
    emit keyValuesChanged();
}

bool ElevationWidgetPrivate::showIndex() const { return input.showIndex; }
void ElevationWidgetPrivate::setShowIndex(bool state) {
    if (input.showIndex == state) return;
    input.showIndex = state;
    emit showIndexChanged();
}

QList<QPointF> ElevationWidgetPrivate::intersections() const { return m_intersections; }
void ElevationWidgetPrivate::setIntersections(const QList<QPointF>& list) {
    if (m_intersections == list) return;
    m_intersections = list;
    emit intersectionsChanged();
}