#pragma once

#include "elevationwidget.hpp"
#include "RouteTools/elevationtools.h"


namespace Elevation {
    class Elevation;
}

class ElevationWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(ElevationWidget)

    public:
        explicit ElevationWidgetPrivate(QObject* parent = nullptr);
        virtual ~ElevationWidgetPrivate() = default;
        ElevationWidget* q_ptr;
        Elevation::Elevation* heightmapParser;
        Elevation::ElevationTools* routeParser;

        Q_INVOKABLE void resize(float w, float h, float zoom_w, float zoom_h = 1);
        Q_INVOKABLE QPointF iterateOverRange(float rangeStart, float rangeStop);

        QGeoPath geopath;

    signals:
        void requestAll();
        void requestPath();
        void requestIntersects();

        void colorsChanged();
        void profileChanged();
        void keyValuesChanged();

    private:
        void recalculate(bool emitFlag = false);
        void recalculateWithGeopathChanged();

    private:
        Q_PROPERTY(QVector<QPointF> profile READ profile WRITE setProfile NOTIFY profileChanged)
        QVector<QPointF> m_profile;
        QVector<QPointF> profile() const;    void setProfile(const QVector<QPointF>& vec);

        Q_PROPERTY(QList<QString> colors READ colors WRITE setColors NOTIFY colorsChanged)
        QList<QString> m_colors = { "#263238", "#dedede", "#607d8b",
                                    "#9ccc65", "#ffab40", "#ff7043" };
        QList<QString> colors() const;       void setColors(const QList<QString>& list);

        Q_PROPERTY(QList<float> keyValues READ keyValues WRITE setKeyValues NOTIFY keyValuesChanged)
        QList<float> m_keyValues;
        QList<float> keyValues() const;      void setKeyValues(const QList<float>& values);


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

        struct Axes {
            struct Axis {
                float maxValue;
                float roundMaxValue;
                int scaleValue;
                float scaleCount;
                float scalePixelSize;
            };
            Axis x, y;
            const float stretch = 1.2;
        }; Axes axis;

        struct Iterator
        {
            int rangeMin = 0;
            int rangeMax = 0;
            int range = 0;
            bool rangeSet = false;
        }; Iterator iterator;
};
