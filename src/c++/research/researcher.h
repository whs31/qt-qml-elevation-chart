/**
  *  @file FILE
  *  @author Dmitry Ryazancev
  *  @date 16.08.23
  *  @copyright Radar-MMS 2023
  */

#pragma once

#include <vector>
#include <QtCore/QObject>
#include <QtCore/QFutureWatcher>
#include <QtCore/QVector>
#include <QtCore/QPointF>
#include <QtPositioning/QGeoPath>
#include "types/elevationpoint.h"
#include "types/intersectionpoint.h"
#include "types/route.h"
#include "types/envelope.h"
#include "types/metrics.h"

using std::vector;

namespace ElevationChart
{
  class Researcher : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged FINAL)

    public:
      struct EnvelopeResult
      {
        Route route;
        vector<ElevationPoint> boundPolygon;
      };

      class EnvelopeLegacyResult
      {
        public:
          EnvelopeLegacyResult() = default;

          QVector<QPointF> groundProfile() const;
          QVector<QPointF> lowBound() const;
          QVector<QPointF> highBound() const;
          QVector<QPointF> routeProfile() const;
          QVector<IntersectionPoint> route() const;

          void addGroundProfilePoint(const QPointF&);
          void addLowBoundPoint(const QPointF&);
          void addHighBoundPoint(const QPointF&);
          void addRouteProfilePoint(const QPointF&);
          void addRoutePoint(const IntersectionPoint&);

        private:
          QVector<QPointF> m_groundProfile;
          QVector<QPointF> m_lowBound;
          QVector<QPointF> m_highBound;
          QVector<QPointF> m_routeProfile;
          QVector<IntersectionPoint> m_route;
      };

      explicit Researcher(QObject* parent = nullptr);

      [[nodiscard]] bool busy() const;

      Q_SLOT void researchIntersections(const QGeoPath& path);
      Q_SLOT void researchEnvelope(const QGeoPath& path, const Metrics& metrics, const Envelope& envelope);
      Q_SLOT void researchEnvelopeLegacy(const QGeoPath& path, const Metrics& metrics, const Envelope& envelope);

      static QGeoPath plotGeopathProfile(const QGeoPath& path);

    signals:
      void busyChanged();

      void researchIntersectionsFinished(vector<ElevationPoint>);
      void researchEnvelopeFinished(EnvelopeResult);
      void researchEnvelopeLegacyFinished(EnvelopeLegacyResult);

    private:
      auto fillProfile(const QList<QGeoCoordinate>& list, const QGeoPath& path) -> vector<IntersectionPoint>;

      static auto createRawGroundPath(const QGeoPath& path) -> vector<IntersectionPoint>;
      static auto createRawGroundPathLegacy(const QGeoPath& path) -> vector<IntersectionPoint>;
      static float angle3point(QPointF a, QPointF b, QPointF c);

    private:
      QFutureWatcher<void> m_watcher;
      QFutureWatcher<void> m_watcher2;
      bool m_busy;
      bool m_busy2;
  };
} // ElevationChart
