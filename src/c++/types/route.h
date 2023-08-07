#pragma once

#include <vector>
#include <QtPositioning/QGeoPath>
#include "routepoint.h"

using std::vector;

namespace ElevationChart
{
  class Route
  {
    public:
      Route();
      explicit Route(const QGeoPath& path, float velocity = 0);
      explicit Route(const vector<RoutePoint>& vec);

      void add(const RoutePoint& point);
      void add(const QGeoCoordinate& coord, float velocity = 0);

      void remove(int index);
      void remove(const RoutePoint& point);

      void replace(int index, const RoutePoint& point);

      void clear();

      void set(const QGeoPath& path, float velocity = 0);
      void set(const vector<RoutePoint>& vec);

      void setVelocity(float velocity);
      void setVelocity(const vector<float>& velocities, float fallback = 0);

      int size() const;
      QGeoPath toGeoPath() const;
      RoutePoint at(int index) const;

      bool contains(const RoutePoint& point);
      bool valid() const;

    public:
      static Route fromGeoPath(const QGeoPath& path, float velocity = 0);
      static Route fromPointsVector(const vector<RoutePoint>& vec);

    private:
      vector<RoutePoint> m_vec;
  };
} // ElevationChart