#pragma once

#include "charts/elevationwidget.hpp"

namespace Elevation {
    class Elevation;
    class ElevationTools;
}

namespace Charts
{
    /// @private
    class ElevationWidgetPrivate : public QObject
    {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ElevationWidget)

        Elevation::Elevation* heightmapParser;
        Elevation::ElevationTools* routeParser;

        public:
            explicit ElevationWidgetPrivate(ElevationWidget* parent);
            virtual ~ElevationWidgetPrivate() = default;

            list<GeoPoint> getRoute();
            void setRoute(const list<GeoPoint>& route, const QGeoCoordinate& uav_position = QGeoCoordinate());
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
            void setEnevelopeCoridorSize(float distance);
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
