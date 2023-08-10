//
// Created by whs31 on 10.08.23.
//

#pragma once
#include <QtCore/QMetaType>

namespace ElevationChart
{
  class Bounds
  {
    Q_GADGET
    Q_PROPERTY(float x READ x WRITE setX)
    Q_PROPERTY(float y READ y WRITE setY)

    public:
      Bounds();
      Bounds(float x, float y);

      [[nodiscard]] float x() const;    void setX(float);
      [[nodiscard]] float y() const;    void setY(float);

    private:
      float m_x;
      float m_y;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::Bounds)

namespace ElevationChart
{
  inline Bounds::Bounds()
    : m_x(0)
    , m_y(0)
  {}

  inline Bounds::Bounds(float x, float y)
    : m_x(x)
    , m_y(y)
  {}

  inline float Bounds::x() const { return m_x; }
  inline void Bounds::setX(float q) { m_x = q; }
  inline float Bounds::y() const { return m_y; }
  inline void Bounds::setY(float q) { m_y = q; }
} // ElevationChart