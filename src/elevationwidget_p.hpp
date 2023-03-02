#pragma once

#include "elevationwidget.hpp"

class ElevationWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(ElevationWidget)

    public:
        explicit ElevationWidgetPrivate(QObject* parent = nullptr);
        virtual ~ElevationWidgetPrivate() = default;
        ElevationWidget* q_ptr;

        Q_INVOKABLE void resize(float w, float h, float zoom_w, float zoom_h = 1);

    signals:
        void colorsChanged();

    private:
        void recalculate();

    private:
        Q_PROPERTY(QList<QString> colors READ colors WRITE setColors NOTIFY colorsChanged)
        QList<QString> m_colors = { "#263238", "#dedede", "607d8b",
                                    "9ccc65", "#ffab40", "#ff7043" };
        QList<QString> colors() const;       void setColors(const QList<QString>& list);

        struct Layout {
            float width = 500;
            float height = 100;
            float horizontal_zoom = 1;
            float vertical_zoom = 1;
        }; Layout layout;

        struct AircraftMetrics {
            float velocity = 75;
            float climbRate = 1;
            float descendRate = 1;
        }; AircraftMetrics aircraftMetrics;

        struct Input {
            bool showIndex = true;
        }; Input input;

};
