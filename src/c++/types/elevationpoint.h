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
       * \param is_valid - валидность точки. <i>По умолчанию - <tt>true</tt></i>.
       * \param is_intersecting - состояние пересечения точки с рельефом. <i>По умолчанию - <tt>false</tt></i>.
       */
      ElevationPoint(float distance_meters, float elevation_meters, bool is_valid = true, bool is_intersecting = false);

      [[nodiscard]] float distance() const;   ///< Возвращает текущее расстояние от нуля в метрах.
      void setDistance(float);                ///< Задает текущее неотрицательное расстояние от нуля в метрах.

      [[nodiscard]] float elevation() const;  ///< Возвращает текущую абсолютную высоту точки в метрах.
      void setElevation(float);               ///< Задает текущую абсолютную высоту точки в метрах.

      [[nodiscard]] bool valid() const;       ///< Возвращает <tt>true</tt>, если точка валидна.
      void setValid(bool);                    ///< Задает валидность точки.

      [[nodiscard]] bool intersects() const;  ///< Возвращает <tt>true</tt>, если точка пересекается с рельефом.
      void setIntersects(bool);               ///< Задает состояние пересечения с рельефом у точки.

    private:
      float m_distance;
      float m_elevation;
      bool m_valid;
      bool m_intersects;
  };

  inline ElevationPoint::ElevationPoint()
    : m_distance(0)
    , m_elevation(0)
    , m_valid(true)
    , m_intersects(false)
  {}

  inline ElevationPoint::ElevationPoint(float distance_meters, float elevation_meters, bool is_valid, bool is_intersecting)
    : m_distance(distance_meters)
    , m_elevation(elevation_meters)
    , m_valid(is_valid)
    , m_intersects(is_intersecting)
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
  inline bool ElevationPoint::valid() const { return m_valid; }
  inline void ElevationPoint::setValid(bool x) { m_valid = x; }
  inline bool ElevationPoint::intersects() const { return m_intersects; }
  inline void ElevationPoint::setIntersects(bool x) { m_intersects = x; }
} // ElevationChart