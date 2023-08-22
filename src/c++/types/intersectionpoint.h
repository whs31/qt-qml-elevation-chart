#pragma once

#include "elevationpoint.h"
#include <QtPositioning/QGeoCoordinate>

namespace ElevationChart
{
  class IntersectionPoint : public ElevationPoint
  {
    public:
      /// \brief Перечисление состояний пересечения с землей.
      enum Intersection
      {
        NonIntersecting,        ///< Точка не пересекается с землей.
        IntersectingIn,         ///< Точка пересекается с землей. Предыдущая точка не пересекалась с землей.
        IntersectingOut,        ///< Точка пересекается с землей. Следующая точка не пересекается с землей.
        InsideGround            ///< Точка лежит внутри рельефа. Соседние точки пересекаются с землей либо лежат внутри рельефа.
      };

      IntersectionPoint();
      IntersectionPoint(float distance, float elevation, bool is_valid, bool is_base, Intersection state, const QGeoCoordinate& coord = QGeoCoordinate());

      [[nodiscard]] bool base() const;                  void setBase(bool);
      [[nodiscard]] Intersection state() const;         void setState(Intersection);
      [[nodiscard]] QGeoCoordinate coordinate() const;  void setCoordinate(const QGeoCoordinate&);

    private:
      bool m_base;
      Intersection m_state;
      QGeoCoordinate m_coordinate;
  };
} // ElevationChart

namespace ElevationChart
{
  /**
   * \class IntersectionPoint
   * \brief Точка на графике профиля высот, хранящая состояние пересечения с рельефом.
   */

  /**
   * \brief Создает новую пустую точку пересечения с рельефом.
   */
  inline IntersectionPoint::IntersectionPoint()
    : ElevationPoint()
    , m_base(true)
    , m_state(NonIntersecting)
    , m_coordinate(QGeoCoordinate())
  {}

  /**
   * \brief Создает точку пересечения с рельефом с указанными параметрами.
   * \param distance - координата Х: расстояние от нуля на ломаной в метрах. Всегда больше нуля.
   * \param elevation - координата Y: высота точки в метрах относительно уровня моря. Может быть как
   * положительной, так и отрицательной.
   * \param is_valid - валидность точки.
   * \param is_base - является ли точка базовой (исходной) точкой пути, или она появилась в результате пересечения с рельефом.
   * \param state - состояние пересечения.
   * \param coord - географическая широта и долгота точки.
   */
  inline IntersectionPoint::IntersectionPoint(float distance, float elevation, bool is_valid, bool is_base, Intersection state, const QGeoCoordinate& coord)
    : ElevationPoint(distance, elevation, is_valid)
    , m_base(is_base)
    , m_state(state)
    , m_coordinate(coord)
  {}

  /// \brief Возвращает <tt>true</tt>, если точка является базовой точкой исходного пути.
  inline bool IntersectionPoint::base() const { return m_base; }

  /// \brief Задает флаг, является ли точка базовой точкой исходного пути.
  inline void IntersectionPoint::setBase(bool x) { m_base = x; }

  /// \brief Возвращает состояние пересечения точки с рельефом.
  inline IntersectionPoint::Intersection IntersectionPoint::state() const { return m_state; }

  /// \brief Задает состояние пересечения точки с рельефом.
  inline void IntersectionPoint::setState(ElevationChart::IntersectionPoint::Intersection x) { m_state = x; }

  /// \brief Возвращает географическую координату точки.
  inline QGeoCoordinate IntersectionPoint::coordinate() const { return m_coordinate; }

  /// \brief Задает географическую координату точки.
  inline void IntersectionPoint::setCoordinate(const QGeoCoordinate& x) { m_coordinate = x; }
} // ElevationChart
