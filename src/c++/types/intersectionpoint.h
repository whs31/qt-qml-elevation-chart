#pragma once

#include "elevationpoint.h"

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
      IntersectionPoint(float distance, float elevation, bool is_valid, bool is_base, Intersection state);

      [[nodiscard]] bool base() const;            void setBase(bool);
      [[nodiscard]] Intersection state() const;   void setState(Intersection);

    private:
      bool m_base;
      Intersection m_state;
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
  {}

  /**
   * \brief Создает точку пересечения с рельефом с указанными параметрами.
   * \param distance - координата Х: расстояние от нуля на ломаной в метрах. Всегда больше нуля.
   * \param elevation - координата Y: высота точки в метрах относительно уровня моря. Может быть как
   * положительной, так и отрицательной.
   * \param is_valid - валидность точки.
   * \param is_base - является ли точка базовой (исходной) точкой пути, или она появилась в результате пересечения с рельефом.
   * \param state - состояние пересечения.
   */
  inline IntersectionPoint::IntersectionPoint(float distance, float elevation, bool is_valid, bool is_base, Intersection state)
    : ElevationPoint(distance, elevation, is_valid)
    , m_base(is_base)
    , m_state(state)
  {}

  /// \brief Возвращает <tt>true</tt>, если точка является базовой точкой исходного пути.
  inline bool IntersectionPoint::base() const { return m_base; }

  /// \brief Задает флаг, является ли точка базовой точкой исходного пути.
  inline void IntersectionPoint::setBase(bool x) { m_base = x; }

  /// \brief Возвращает состояние пересечения точки с рельефом.
  inline IntersectionPoint::Intersection IntersectionPoint::state() const { return m_state; }

  /// \brief Задает состояние пересечения точки с рельефом.
  inline void IntersectionPoint::setState(ElevationChart::IntersectionPoint::Intersection x) { m_state = x; }
} // ElevationChart
