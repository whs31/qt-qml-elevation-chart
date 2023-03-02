#pragma once

#include "elevationwidget.hpp"

class ElevationWidgetPrivate
{
    Q_DECLARE_PUBLIC(ElevationWidget)

    public:
        ElevationWidgetPrivate() = default;
        virtual ~ElevationWidgetPrivate() = default;
        ElevationWidget* q_ptr;

};
