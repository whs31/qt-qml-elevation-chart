/**
  *  @file FILE
  *  @author Dmitry Ryazancev
  *  @date 16.08.23
  *  @copyright Radar-MMS 2023
  */

#pragma once

#include <vector>
#include <QtCore/QObject>
#include <QtPositioning/QGeoPath>
#include "types/intersectionpoint.h"

using std::vector;

namespace ElevationChart
{
  class Researcher : public QObject
  {
    Q_OBJECT

    public:
      explicit Researcher(QObject* parent = nullptr);

      Q_SLOT void researchIntersections(const QGeoPath& path);

      static QGeoPath plotGeopathProfile(const QGeoPath& path);
      static float altitudeAtDistance(const QGeoCoordinate& a, const QGeoCoordinate& b, int distance);

    signals:
      void researchIntersectionsFinished(vector<ElevationPoint>);
  };
} // ElevationChart
