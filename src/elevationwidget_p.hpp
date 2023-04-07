#pragma once

#include "charts/elevationwidget.hpp"
#include <QGeoPath>

namespace Elevation {
    class Elevation;
    class ElevationTools;
    class RouteAndElevationProfiles;
    class Point;
}
namespace ChartsOpenGL {
    class CDeclarativePolyline;
    class CDeclarativePolygon;
}

using std::list;

namespace Charts
{
    class PointsModel;
    /// @private
    class ElevationWidgetPrivate : public QObject
    {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ElevationWidget)

        Elevation::Elevation* heightmapParser;
        Elevation::ElevationTools* routeParser;
        PointsModel* model;

        // variables
        list<GeoPoint> m_route;
        QGeoPath m_envelope;
        QGeoPath m_metricsPath;

        QGeoCoordinate m_uavPosition;

        bool m_valid = true;
        bool m_matchingMetrics = true;
        bool m_intersects = false;

        struct AircraftMetrics {
            float velocity = 75;
            float climbRate = 1;
            float descendRate = 1;
            float envelopeHeight = 100;
            float envelopeSize = 100;
        } aircraftMetrics;

        struct Input {
            bool showIndex = true;
        } input;

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
            float relative_height = 0;
        } axis;

        // qml instances
        ChartsOpenGL::CDeclarativePolyline* m_pathPolyline = nullptr;
        ChartsOpenGL::CDeclarativePolygon* m_profilePolygon = nullptr;
        ChartsOpenGL::CDeclarativePolyline* m_metricsPolyline = nullptr;
        ChartsOpenGL::CDeclarativePolyline* m_envelopePolyline = nullptr;
        ChartsOpenGL::CDeclarativePolygon* m_intersectsPolygon = nullptr;

        public:
            explicit ElevationWidgetPrivate(ElevationWidget* parent);
            virtual ~ElevationWidgetPrivate() = default;
            void linkWithQML(QQuickItem* rootObject);

            list<GeoPoint> getRoute();
            void setRoute(const list<GeoPoint>& route);
            void setUAVPosition(const QGeoCoordinate& position);
            void setUAVPosition(double latitude, double longitude);
            bool isIntersecting();
            bool isValid();
            void setClimbRate(float rate);
            void setDescendRate(float rate);
            void setGlobalVelocity(float velocity);
            void applyMetricsCorrection();
            bool isMatchingMetrics();
            void setEnvelopeMinimumAltitude(float altitude);
            void setEnvelopeCoridorSize(float distance);
            void estimateEnvelope();
            void applyEnvelopeCorrection();

        protected:
            ElevationWidget* q_ptr;

        private:
            // common + profile
            enum UpdateMode
            {
                RebuildProfile,
                KeepProfile
            };
            void update(UpdateMode mode, float force_y_axis_height = 0);
            void sync(QVector<QPointF> vec);

            // points
            private slots:
                void syncPointsWithPath(int _index);

            // metrics
            void calculateMetrics();

            // envelope
            void calculateEnvelope();
            private slots:
                void calculateEnvelopeFinished(quint8 progress, const Elevation::RouteAndElevationProfiles& deltaResult);

            // intersects
            void calculateIntersects();
            private slots:
                void calculateIntersectsFinished(quint8 progress, const QVector<Elevation::Point>& resultPath);

            // shared
            QPointF toPixelCoords(const QPointF& point, float x_max, float y_max, float y_stretch, float pixel_width, float pixel_height);
            QPointF fromPixelCoords(const QPointF& point, float x_max, float y_max, float y_stretch, float pixel_width, float pixel_height);
            list<GeoPoint> toRoute(const QGeoPath& path);
            QGeoPath fromRoute(const list<GeoPoint> route);
    };
} ///namespace Charts;

inline void initialize_qrc_file_within_namespace_1(const char* comment)
{
    Q_INIT_RESOURCE(charts);
}
