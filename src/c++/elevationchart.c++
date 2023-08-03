//
// Created by whs31 on 03.08.23.
//

#include "elevationchart.h"

namespace ElevationChart
{
  ChartItem::ChartItem(QQuickItem* parent)
    : QQuickItem(parent)
    , m_palette(new SG::BasicPalette(this))
  {}

  SG::BasicPalette* ChartItem::palette() const { return m_palette; }
} // ElevationChart