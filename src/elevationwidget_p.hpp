#pragma once

#include "elevationwidget.hpp"

class ElevationWidgetPrivate : QObject
{
    Q_DECLARE_PUBLIC(ElevationWidget)

    public:
        explicit ElevationWidgetPrivate(QObject* parent = nullptr);
        virtual ~ElevationWidgetPrivate() = default;
        ElevationWidget* q_ptr;

        Q_INVOKABLE void resize(float w, float h, float zoom_w, float zoom_h = 1);

    private:
        void recalculate();

    private:
        struct Layout {
            float width = -1;
            float height = -1;
            float horizontal_zoom = 1;
            float vertical_zoom = 1;
        }; Layout layout;

        struct AircraftMetrics {
            float velocity = -1;
            float climbRate = -1;
            float descendRate = -1;
        }; AircraftMetrics aircraftMetrics;

        struct Input {
            bool showIndex = true;
        }; Input input;
};
