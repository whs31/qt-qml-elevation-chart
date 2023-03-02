#include "elevationwidget.hpp"
#include "elevationwidget_p.hpp"

#include <qqml.h>
#include <QDebug>

ElevationWidget::ElevationWidget(QObject *parent)
    : QObject{parent}
    , d_ptr(new ElevationWidgetPrivate(this))
{
    qmlRegisterSingletonInstance("ElevationWidgetImpl", 1, 0, "Impl", d_ptr);
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
    // recalculate()
}

void ElevationWidget::setClimbRate(float rate)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.climbRate = rate;
    // recalculate()
}

void ElevationWidget::setDescendRate(float rate)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.descendRate = rate;
    // recalculate()
}

void ElevationWidget::setPallete(QString backgroundColor, QString foregroundColor, QString chartColor,
                                 QString successColor, QString warningColor, QString errorColor)
{
    Q_D(ElevationWidget);
    d->pallete.background = backgroundColor;
    d->pallete.foreground = foregroundColor;
    d->pallete.chart = chartColor;
    d->pallete.success = successColor;
    d->pallete.warning = warningColor;
    d->pallete.critical = errorColor;
    // updatePallete();
}

//=== === === === === === === === === === === === //
//                                                //
//      💩🖼️🖥️📥 Private implementation 🛠️📥😀❌      //
//                                                //
//=== === === === === === === === === === === === //

ElevationWidgetPrivate::ElevationWidgetPrivate(QObject* parent)
    : QObject{parent}
{

}

void ElevationWidgetPrivate::resize(float w, float h, float zoom_w, float zoom_h)
{
    if(w == layout.width && h == layout.height &&
       zoom_w == layout.horizontal_zoom && zoom_h == layout.vertical_zoom)
        return;
}
