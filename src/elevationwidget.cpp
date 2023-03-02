#include "elevationwidget.hpp"
#include "elevationwidget_p.hpp"

ElevationWidget::ElevationWidget(QObject *parent)
    : QObject{parent}
    , d_ptr(new ElevationWidgetPrivate())
{

}

