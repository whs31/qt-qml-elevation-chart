#pragma once

namespace ElevationChart
{
  class ElevationPoint
  {
    public:
      ElevationPoint();
      ElevationPoint(float distance_meters, float elevation_meters, bool is_valid = true);

      [[nodiscard]] float distance() const;     void setDistance(float);
      [[nodiscard]] float elevation() const;    void setElevation(float);
      [[nodiscard]] bool valid() const;         void setValid(bool);

    private:
      float m_distance;
      float m_elevation;
      bool m_valid;
  };

  /**
   * \class ElevationPoint
   * \brief Точка профиля высот.
   * \details Двумерная точка в пространстве, хранящая координаты в виде
   * расстояния и высоты.
   */

  /// \brief Создает точку с координатами <tt>[0, 0]</tt>.
  inline ElevationPoint::ElevationPoint()
    : m_distance(0)
    , m_elevation(0)
    , m_valid(true)
  {}

  /**
   * \brief Создает точку с указанными координатами.
   * \param distance_meters - координата Х: расстояние от нуля на ломаной в метрах. Всегда больше нуля.
   * \param elevation_meters - координата Y: высота точки в метрах относительно уровня моря. Может быть как
   * положительной, так и отрицательной.
   * \param is_valid - валидность точки. <i>По умолчанию - <tt>true</tt></i>.
   * \param is_intersecting - состояние пересечения точки с рельефом. <i>По умолчанию - <tt>false</tt></i>.
   */
  inline ElevationPoint::ElevationPoint(float distance_meters, float elevation_meters, bool is_valid)
    : m_distance(distance_meters)
    , m_elevation(elevation_meters)
    , m_valid(is_valid)
  {
    if(m_distance < 0)
      m_distance = 0;
  }

  /// \brief Возвращает текущее расстояние от нуля в метрах.
  inline float ElevationPoint::distance() const { return m_distance; }

  /// \brief Задает текущее неотрицательное расстояние от нуля в метрах.
  inline void ElevationPoint::setDistance(float x) {
    if(x < 0)
      m_distance = 0;
    m_distance = x;
  }

  /// \brief Возвращает текущую абсолютную высоту точки в метрах.
  inline float ElevationPoint::elevation() const { return m_elevation; }

  /// \brief Задает текущую абсолютную высоту точки в метрах.
  inline void ElevationPoint::setElevation(float x) { m_elevation = x; }

  /// \brief Возвращает <tt>true</tt>, если точка валидна.
  inline bool ElevationPoint::valid() const { return m_valid; }

  /// \brief Задает валидность точки.
  inline void ElevationPoint::setValid(bool x) { m_valid = x; }
} // ElevationChart