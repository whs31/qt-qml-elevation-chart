#pragma once

namespace ElevationChart
{
  /**
   * \brief Точка профиля высот.
   * \details Двумерная точка в пространстве, хранящая координаты в виде
   * расстояния и высоты.
   */
  class ElevationPoint
  {
    public:
      /// \brief Создает точку с координатами <tt>[0, 0]</tt>.
      ElevationPoint();

      /**
       * \brief Создает точку с указанными координатами.
       * \param distance_meters - координата Х: расстояние от нуля на ломаной в метрах. Всегда больше нуля.
       * \param elevation_meters - координата Y: высота точки в метрах относительно уровня моря. Может быть как
       * положительной, так и отрицательной.
       */
      ElevationPoint(float distance_meters, float elevation_meters);

      [[nodiscard]] float distance() const;   ///< Возвращает текущее расстояние от нуля в метрах.
      void setDistance(float);                ///< Задает текущее неотрицательное расстояние от нуля в метрах.

      [[nodiscard]] float elevation() const;  ///< Возвращает текущую абсолютную высоту точки в метрах.
      void setElevation(float);               ///< Задает текущую абсолютную высоту точки в метрах.

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
  {
    if(m_distance < 0)
      m_distance = 0;
  }

  inline float ElevationPoint::distance() const { return m_distance; }
  inline void ElevationPoint::setDistance(float x) {
    if(x < 0)
      m_distance = 0;
    m_distance = x;
  }
  inline float ElevationPoint::elevation() const { return m_elevation; }
  inline void ElevationPoint::setElevation(float x) { m_elevation = x; }
} // ElevationChart