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
    Q_PROPERTY(float x READ x WRITE setX FINAL)
    Q_PROPERTY(float y READ y WRITE setY FINAL)
    Q_PROPERTY(float stretch READ stretch WRITE setStretch FINAL)

    public:
      Bounds();
      Bounds(float x, float y, float stretch = 1.15f);

      [[nodiscard]] float x() const;        void setX(float);
      [[nodiscard]] float y() const;        void setY(float);
      [[nodiscard]] float stretch() const;  void setStretch(float);

    private:
      float m_x;
      float m_y;
      float m_stretch;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::Bounds)

namespace ElevationChart
{
  inline Bounds::Bounds()
    : m_x(0)
    , m_y(0)
    , m_stretch(1.15f)
  {}

  inline Bounds::Bounds(float x, float y, float stretch)
    : m_x(x)
    , m_y(y)
    , m_stretch(1.15f)
  {}

  inline float Bounds::x() const { return m_x; }
  inline void Bounds::setX(float q) { m_x = q; }
  inline float Bounds::y() const { return m_y; }
  inline void Bounds::setY(float q) { m_y = q; }
  inline float Bounds::stretch() const { return m_stretch; }
  inline void Bounds::setStretch(float q) { m_stretch = q; }
} // ElevationChart