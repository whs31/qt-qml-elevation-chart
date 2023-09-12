//
// Created by whs31 on 06.09.23.
//

#pragma once
#include <LPVL/BasicPalette>

namespace ElevationChart
{
  class Palette : public LPVL::BasicPalette
  {
    Q_GADGET
    Q_PROPERTY(QColor corridor READ corridor WRITE setCorridor FINAL)
    Q_PROPERTY(QColor uav READ uav WRITE setUav FINAL)

    public:
      Palette();
      [[nodiscard]] QColor corridor() const; void setCorridor(const QColor&);
      [[nodiscard]] QColor uav() const;      void setUav(const QColor&);

    private:
      QColor m_corridor;
      QColor m_uav;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::Palette)

namespace ElevationChart
{
  inline Palette::Palette()
    : LPVL::BasicPalette()
    , m_corridor("blue")
    , m_uav("pink")
  {}

  inline QColor Palette::corridor() const { return m_corridor; }
  inline void Palette::setCorridor(const QColor& x) { m_corridor = x; }

  inline QColor Palette::uav() const { return m_uav; }
  inline void Palette::setUav(const QColor& x) { m_uav = x; }
} // ElevationChart