#pragma once

#include <vector>
#include <QtPositioning/QGeoPath>
#include "routepoint.h"
#include "elevationpoint.h"
#define invokable Q_INVOKABLE

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

      invokable void add(const ElevationChart::RoutePoint& point);
      invokable void add(const QGeoCoordinate& coord, float velocity = 0);
      invokable void remove(int index);
      invokable void remove(const ElevationChart::RoutePoint& point);
      invokable void replace(int index, const ElevationChart::RoutePoint& point);
      invokable void clear();

      invokable void set(const QGeoPath& path, float velocity = 0);
      void set(const vector<RoutePoint>& vec);
      invokable void setVelocity(float velocity);
      invokable void setVelocity(const vector<float>& velocities, float fallback = 0);

      invokable [[nodiscard]] QGeoPath toGeoPath() const;
      invokable [[nodiscard]] QVariantList toVariantList() const;
      [[nodiscard]] auto toElevationGraph() const -> vector<ElevationPoint>;
      [[nodiscard]] auto velocities() const -> vector<float>;

      invokable [[nodiscard]] int size() const;
      invokable ElevationChart::RoutePoint& at(int index);
      invokable [[nodiscard]] bool contains(const RoutePoint& point);
      invokable [[nodiscard]] bool valid() const;


    public:
      static Route fromGeoPath(const QGeoPath& path, float velocity = 0);
      static Route fromPointsVector(const vector<RoutePoint>& vec);

    private:
      vector<RoutePoint> m_vec;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::Route)