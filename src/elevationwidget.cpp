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

//█████████████████████████████████ 🔒 PRIVATE IMPLEMENTATION 🔒 █████████████████████████████████████──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────████████


ElevationWidgetPrivate::ElevationWidgetPrivate(QObject* parent)
    : QObject{parent}
{

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
    // recalculate (all) ()
}

QList<QString> ElevationWidgetPrivate::colors() const { return m_colors; }
void ElevationWidgetPrivate::setColors(const QList<QString>& list) {
    if (m_colors == list) return;
    m_colors = list;
    emit colorsChanged();
}
