#pragma once

#include <QtPositioning/QGeoCoordinate>

namespace ElevationChart
{
  class RoutePoint
  {
    public:
      RoutePoint();
      RoutePoint(double latitude, double longitude, float elevation = 0, float velocity_ms = 0);
      explicit RoutePoint(const QGeoCoordinate& coord, float velocity_ms = 0);

      [[nodiscard]] QGeoCoordinate coordinate() const;
      void setCoordinate(const QGeoCoordinate&);

      [[nodiscard]] double latitude() const;
      void setLatitude(double);

      [[nodiscard]] double longitude() const;
      void setLongitude(double);

      [[nodiscard]] float altitude() const;
      void setAltitude(float);

      [[nodiscard]] float velocity() const;
      void setVelocity(float);

    private:
      QGeoCoordinate m_coordinate;
      float m_velocity;
  };

  inline RoutePoint::RoutePoint()
    : m_coordinate({0, 0, 0})
    , m_velocity(0)
  {}

  inline RoutePoint::RoutePoint(double latitude, double longitude, float elevation, float velocity_ms)
    : m_coordinate(latitude, longitude, elevation)
    , m_velocity(velocity_ms)
  {}

  inline RoutePoint::RoutePoint(const QGeoCoordinate& coord, float velocity_ms)
    : m_coordinate(coord)
    , m_velocity(velocity_ms)
  {}

  inline QGeoCoordinate RoutePoint::coordinate() const { return m_coordinate; }
  inline void RoutePoint::setCoordinate(const QGeoCoordinate& x) { m_coordinate = x; }
  inline double RoutePoint::latitude() const { return m_coordinate.latitude(); }
  inline void RoutePoint::setLatitude(double x) { m_coordinate.setLatitude(x); }
  inline double RoutePoint::longitude() const { return m_coordinate.longitude(); }
  inline void RoutePoint::setLongitude(double x) { m_coordinate.setLongitude(x); }
  inline float RoutePoint::altitude() const { return static_cast<float>(m_coordinate.altitude()); }
  inline void RoutePoint::setAltitude(float x) { m_coordinate.setAltitude(x); }
  inline float RoutePoint::velocity() const { return m_velocity; }
  inline void RoutePoint::setVelocity(float x) { if(not qIsNaN(x)) m_velocity = x; }
} // ElevationChart