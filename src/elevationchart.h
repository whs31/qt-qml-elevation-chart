#pragma once

#include <vector>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtQuick/QQuickItem>
#include <QtPositioning/QGeoCoordinate>
#include <LPVL/ChartBase>
#include <LPVL/GLGeometry>

using std::vector;

namespace Widgets
{
    class ElevationChart : public LPVL::ChartBase
    {
        struct ChartPoint
        {
            QGeoCoordinate toQGeoCoordinate() const noexcept;

            double latitude;
            double longitude;
            float altitude;
            float velocity;
        };

        Q_OBJECT
        Q_PROPERTY(QGeoCoordinate uavPosition READ uavPosition WRITE setUavPosition NOTIFY uavPositionChanged)
        Q_PROPERTY(bool intersecting READ intersecting WRITE setIntersecting NOTIFY intersectingChanged)
        Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)

        Q_PROPERTY(float climbRate READ climbRate WRITE setClimbRate NOTIFY climbRateChanged)
        Q_PROPERTY(float descendRate READ descendRate WRITE setDescendRate NOTIFY descendRateChanged)
        Q_PROPERTY(float fallbackVelocity READ fallbackVelocity WRITE setFallbackVelocity NOTIFY fallbackVelocityChanged)
        Q_PROPERTY(bool matchingMetrics READ matchingMetrics WRITE setMatchingMetrics NOTIFY matchingMetricsChanged)

        Q_PROPERTY(float envelopeMinAltitude READ envelopeMinAltitude WRITE setEnvelopeMinAltitude NOTIFY envelopeMinAltitudeChanged)
        Q_PROPERTY(float envelopeCoridorSize READ envelopeCoridorSize WRITE setEnvelopeCoridorSize NOTIFY envelopeCoridorSizeChanged)

        public:
            ElevationChart(QQuickItem* parent = nullptr);

            /*! @brief setPolyline
             *  @details
             *  @param route
             *  @param velocities
             *  @throws std::invalid_argument, если route.size() != velocities.size()
             *          and velocities.size() != 0. !*/
            Q_INVOKABLE void setPolyline(const QList<QVariant>& route, const QList<float> velocities = {});
            Q_INVOKABLE QList<QVariant> getPolyline() const noexcept;
            Q_INVOKABLE QList<float> getVelocities() const noexcept;

            Q_INVOKABLE void applyMetricsCorrection() noexcept;
            Q_INVOKABLE void estimateEnvelope() noexcept;
            Q_INVOKABLE void applyEnvelopeCorrection() noexcept;

            QGeoCoordinate uavPosition() const; void setUavPosition(const QGeoCoordinate&);
            bool intersecting() const; void setIntersecting(bool);
            bool valid() const; void setValid(bool);
            float climbRate() const; void setClimbRate(float);
            float descendRate() const; void setDescendRate(float);
            float fallbackVelocity() const; void setFallbackVelocity(float);
            bool matchingMetrics() const; void setMatchingMetrics(bool);
            float envelopeMinAltitude() const; void setEnvelopeMinAltitude(float);
            float envelopeCoridorSize() const; void setEnvelopeCoridorSize(float);

            signals:
                void polylineChanged();
                void uavPositionChanged();
                void intersectingChanged();
                void validChanged();
                void climbRateChanged();
                void descendRateChanged();
                void fallbackVelocityChanged();
                void matchingMetricsChanged();
                void envelopeMinAltitudeChanged();
                void envelopeCoridorSizeChanged();

        protected:
            QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;

        private:
            void requestUpdate();

        private:
            vector<ChartPoint> m_route;
            vector<LPVL::Point2D> m_profile;

            QGeoCoordinate m_uavPosition;
            bool m_intersecting;
            bool m_valid;
            float m_climbRate;
            float m_descendRate;
            float m_fallbackVelocity;
            bool m_matchingMetrics;
            float m_envelopeMinAltitude;
            float m_envelopeCoridorSize;
    };
} // Widgets
