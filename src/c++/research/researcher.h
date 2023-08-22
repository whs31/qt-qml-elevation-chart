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
#include <QtPositioning/QGeoPath>
#include "types/elevationpoint.h"
#include "types/route.h"
#include "types/envelope.h"
#include "types/metrics.h"

using std::vector;

namespace ElevationChart
{
  class Researcher : public QObject
  {
    struct EnvelopeResult
    {
      Route route;
      vector<ElevationPoint> boundPolygon;
    };

    Q_OBJECT
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged FINAL)

    public:
      explicit Researcher(QObject* parent = nullptr);

      [[nodiscard]] bool busy() const;      void setBusy(bool);

      Q_SLOT void researchIntersections(const QGeoPath& path);
      Q_SLOT void researchEnvelope(const QGeoPath& path, const Metrics& metrics, const Envelope& envelope);

      static QGeoPath plotGeopathProfile(const QGeoPath& path);

    signals:
      void busyChanged();

      void researchIntersectionsFinished(vector<ElevationPoint>);
      void researchEnvelopeFinished(EnvelopeResult);

    private:
      void suspendIntersections(const QGeoPath& path);

    private:
      QFutureWatcher<void> m_watcher;
      bool m_busy;
      QGeoPath m_suspended;
  };
} // ElevationChart
