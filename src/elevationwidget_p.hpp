#pragma once

#include "elevationwidget.hpp"
#include "RouteTools/elevationtools.h"

namespace Elevation {
    class Elevation;
}

/// @private
class ElevationWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(ElevationWidget)

    public:
        explicit ElevationWidgetPrivate(ElevationWidget* parent);
        virtual ~ElevationWidgetPrivate() = default;
        ElevationWidget* q_ptr;
        Elevation::Elevation* heightmapParser;
        Elevation::ElevationTools* routeParser;

        Q_INVOKABLE void resize(float w, float h, float zoom_w, float zoom_h = 1);
        Q_INVOKABLE QPointF iterateOverRange(float rangeStart, float rangeStop);
        Q_INVOKABLE void changeFlightPointAltitude(int index, qreal delta);

        QGeoPath geopath;
        QGeoPath metricsCorrectedGeopath;

        const int32_t ALTITUDE_MIN = 5; // m
        const int32_t ALTITUDE_MAX = 15'000; // m

        public slots:
            void intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point> &resultPath);
            void routeToolsCalculationFinished(quint8 progress, const Elevation::RouteAndElevationProfiles &deltaResult);

        signals:
            void requestAll();
            void requestPath();
            void requestIntersects();
            void requestBounds();

            void colorsChanged();
            void profileChanged();
            void keyValuesChanged();
            void showIndexChanged();
            void pathChanged();
            void intersectionsChanged();
            void correctedPathChanged();
            //void boundsChanged();
            void fileIntegrityChanged();
            void validChanged();

    private:
        QPointF toPixel(const QPointF& point);
        void recalculate(bool emitFlag = false);
        void recalculateWithGeopathChanged();
        void recalculateEnvelope();
//        void recalculateBound(bool slow = false);

        void calculatePath();
        void calculateCorrectedPath();
        void calculateCorrectedPathForUI(QGeoPath c_geopath);
//        void calculateBoundsOffset();

    private:
        Q_PROPERTY(QVector<QPointF> profile READ profile WRITE setProfile NOTIFY profileChanged)
        QVector<QPointF> m_profile;
        QVector<QPointF> profile() const;    void setProfile(const QVector<QPointF>& vec);

        Q_PROPERTY(QList<QPointF> path READ path WRITE setPath NOTIFY pathChanged)
        QList<QPointF> m_path;
        QList<QPointF> path() const;         void setPath(const QList<QPointF>& list);

        Q_PROPERTY(QList<QPointF> correctedPath READ correctedPath WRITE setCorrectedPath NOTIFY correctedPathChanged)
        QList<QPointF> m_correctedPath;
        QList<QPointF> correctedPath() const;void setCorrectedPath(const QList<QPointF>& list);

        Q_PROPERTY(QList<QPointF> intersections READ intersections WRITE setIntersections NOTIFY intersectionsChanged)
        QList<QPointF> m_intersections;
        QList<QPointF> intersections() const;void setIntersections(const QList<QPointF>& list);

//        Q_PROPERTY(QList<QPointF> bounds READ bounds WRITE setBounds NOTIFY boundsChanged)
//        QList<QPointF> m_bounds;             QList<QPointF> m_cleanBounds;
//        QList<QPointF> bounds() const;       void setBounds(const QList<QPointF>& list);

        Q_PROPERTY(QList<QString> colors READ colors WRITE setColors NOTIFY colorsChanged)
        QList<QString> m_colors = { "#000000", "#ffffff", "#888888",
                                    "#00ff00", "#ffff00", "#ff0000" };
        QList<QString> colors() const;       void setColors(const QList<QString>& list);

        Q_PROPERTY(QList<float> keyValues READ keyValues WRITE setKeyValues NOTIFY keyValuesChanged)
        QList<float> m_keyValues;
        QList<float> keyValues() const;      void setKeyValues(const QList<float>& values);

        Q_PROPERTY(bool showIndex READ showIndex WRITE setShowIndex NOTIFY showIndexChanged)
        bool showIndex() const;              void setShowIndex(bool state);

        Q_PROPERTY(bool fileIntegrity READ fileIntegrity WRITE setFileIntegrity NOTIFY fileIntegrityChanged)
        bool m_fileIntegrity = true;
        bool fileIntegrity() const;          void setFileIntegrity(bool state);

        Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)
        bool m_valid = true;
        bool valid() const;          void setValid(bool state);

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
            const float stretch = 1.15;
        }; Axes axis;

        struct Iterator
        {
            int rangeMin = 0;
            int rangeMax = 0;
            int range = 0;
            bool rangeSet = false;
        }; Iterator iterator;

        bool m_isIntersecting = false;
        bool m_isMatchingMetrics = true;

        /// @brief Вектор для скоростей, индивидуальных для каждой точки)
        std::vector<uint8_t> m_speeds;
};
