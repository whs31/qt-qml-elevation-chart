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

    public:
      Palette();
      [[nodiscard]] QColor corridor() const; void setCorridor(const QColor&);

    private:
      QColor m_corridor;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::Palette)

namespace ElevationChart
{
  inline Palette::Palette()
    : LPVL::BasicPalette()
    , m_corridor("blue")
  {}

  inline QColor Palette::corridor() const { return m_corridor; }
  inline void Palette::setCorridor(const QColor& x) { m_corridor = x; }
} // ElevationChart