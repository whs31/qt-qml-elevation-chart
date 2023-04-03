#pragma once

#include "charts/elevationwidget.hpp"

namespace Elevation {
    class Elevation;
    class ElevationTools;
}
namespace ChartsOpenGL {
    class CDeclarativePolyline;
    class CDeclarativePolygon;
}

using std::list;

namespace Charts
{
    /// @private
    class ElevationWidgetPrivate : public QObject
    {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ElevationWidget)

        Elevation::Elevation* heightmapParser;
        Elevation::ElevationTools* routeParser;

        // variables

        list<GeoPoint> m_route;
        QGeoCoordinate m_uavPosition;

        // qml instances

        ChartsOpenGL::CDeclarativePolyline* m_pathPolyline = nullptr;

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

    };
} ///namespace Charts;

inline void initialize_qrc_file_within_namespace_1(const char* comment)
{
    Q_INIT_RESOURCE(charts);
}
