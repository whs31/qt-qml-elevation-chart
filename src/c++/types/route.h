#pragma once

#include <vector>
#include <QtPositioning/QGeoPath>
#include "routepoint.h"
#include "elevationpoint.h"

using std::vector;

namespace ElevationChart
{
  class Route
  {
    Q_GADGET

    public:
      Route();
      explicit Route(const QGeoPath& path, float velocity = 0);
      explicit Route(const vector<RoutePoint>& vec);

      Q_INVOKABLE void add(const ElevationChart::RoutePoint& point);
      Q_INVOKABLE void add(const QGeoCoordinate& coord, float velocity = 0);
      Q_INVOKABLE void remove(int index);
      Q_INVOKABLE void remove(const ElevationChart::RoutePoint& point);
      Q_INVOKABLE void replace(int index, const ElevationChart::RoutePoint& point);
      Q_INVOKABLE void clear();

      Q_INVOKABLE void set(const QGeoPath& path, float velocity = 0);
      void set(const vector<RoutePoint>& vec);
      Q_INVOKABLE void setVelocity(float velocity);
      Q_INVOKABLE void setVelocity(const vector<float>& velocities, float fallback = 0);

      Q_INVOKABLE [[nodiscard]] QGeoPath toGeoPath() const;
      Q_INVOKABLE [[nodiscard]] QVariantList toVariantList() const;
      [[nodiscard]] auto toElevationGraph() const -> vector<ElevationPoint>;
      [[nodiscard]] auto velocities() const -> vector<float>;

      Q_INVOKABLE [[nodiscard]] int size() const;
      Q_INVOKABLE [[nodiscard]] RoutePoint& at(int index);
      Q_INVOKABLE [[nodiscard]] bool contains(const RoutePoint& point);
      Q_INVOKABLE [[nodiscard]] bool valid() const;


    public:
      static Route fromGeoPath(const QGeoPath& path, float velocity = 0);
      static Route fromPointsVector(const vector<RoutePoint>& vec);

    private:
      vector<RoutePoint> m_vec;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::Route)