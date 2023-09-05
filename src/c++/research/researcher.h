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

      explicit Researcher(QObject* parent = nullptr);

      [[nodiscard]] bool busy() const;

      Q_SLOT void researchIntersections(const QGeoPath& path);
      Q_SLOT void researchEnvelope(const QGeoPath& path, const Metrics& metrics, const Envelope& envelope);

      static QGeoPath plotGeopathProfile(const QGeoPath& path);

    signals:
      void busyChanged();

      void researchIntersectionsFinished(vector<ElevationPoint>);
      void researchEnvelopeFinished(EnvelopeResult);

    private:
      static auto fillProfile(const QList<QGeoCoordinate>& list, const QGeoPath& path) -> vector<IntersectionPoint>;
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
