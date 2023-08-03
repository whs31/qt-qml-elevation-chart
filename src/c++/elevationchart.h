//
// Created by whs31 on 03.08.23.
//

#pragma once
#include <QtQuick/QQuickItem>
#include <SG/BasicPalette>

namespace ElevationChart
{
  class ChartItem : public QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(SG::BasicPalette* palette READ palette CONSTANT)

    public:
      explicit ChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] SG::BasicPalette* palette() const;

    protected:

    private:
      SG::BasicPalette* m_palette;
  };
} // ElevationChart
