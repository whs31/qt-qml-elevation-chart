//
// Created by whs31 on 03.08.23.
//

#pragma once
#include <QtQuick/QQuickItem>
#include <SG/BasicPalette>

class QSGGeometryNode;

namespace ElevationChart
{
  class ChartItem : public QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(SG::BasicPalette palette READ palette WRITE setPalette NOTIFY paletteChanged)

    public:
      explicit ChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] SG::BasicPalette palette() const; void setPalette(SG::BasicPalette);

    signals:
      void paletteChanged();

    protected:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;

    protected:
      bool m_require_recolor;
      QSGGeometryNode* m_background_node;

    private:
      SG::BasicPalette m_palette;
  };
} // ElevationChart
