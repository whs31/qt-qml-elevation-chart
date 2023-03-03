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
    d->input.showIndex = state;
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

    recalculateWithGeopathChanged();
}

// █ calculate axes and scales
void ElevationWidgetPrivate::recalculate(bool emitFlag)
{
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

    qDebug() << keyValues();
    if(emitFlag || profile().isEmpty())
        emit requestAll();
    else
        emit requestPath();
}


// █ build profile
void ElevationWidgetPrivate::recalculateWithGeopathChanged()
{
    QVector<QPointF> points = heightmapParser->buildGroundProfileForChart(geopath);
    for (QPointF& point : points) {
        point.setX(point.x() * layout.horizontal_zoom);
        point.setY(layout.height - point.y());
    }
    setProfile(points);
    recalculate(true);
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

QList<float> ElevationWidgetPrivate::keyValues() const { return m_keyValues; }
void ElevationWidgetPrivate::setKeyValues(const QList<float>& values) {
    if (m_keyValues == values) return;
    m_keyValues = values;
    emit keyValuesChanged();
}
