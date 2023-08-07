#pragma once

namespace ElevationChart
{
  class ElevationPoint
  {
    public:
      ElevationPoint();
      ElevationPoint(float distance_meters, float elevation_meters);

      [[nodiscard]] float distance() const;  void setDistance(float);
      [[nodiscard]] float elevation() const; void setElevation(float);

    private:
      float m_distance;
      float m_elevation;
  };

  inline ElevationPoint::ElevationPoint()
    : m_distance(0)
    , m_elevation(0)
  {}

  inline ElevationPoint::ElevationPoint(float distance_meters, float elevation_meters)
    : m_distance(distance_meters)
    , m_elevation(elevation_meters)
  {}

  inline float ElevationPoint::distance() const { return m_distance; }
  inline void ElevationPoint::setDistance(float x) { m_distance = x; }
  inline float ElevationPoint::elevation() const { return m_elevation; }
  inline void ElevationPoint::setElevation(float x) { m_elevation = x; }
} // ElevationChart