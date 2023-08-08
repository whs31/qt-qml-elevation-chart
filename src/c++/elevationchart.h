//
// Created by whs31 on 03.08.23.
//

#pragma once
#include <QtQuick/QQuickItem>
#include <SG/BasicPalette>
#include "types/route.h"

class QSGGeometryNode;

namespace ElevationChart
{
  class ChartItem : public QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(SG::BasicPalette palette READ palette WRITE setPalette NOTIFY paletteChanged FINAL)
    Q_PROPERTY(bool intersecting READ intersecting WRITE setIntersecting NOTIFY intersectingChanged FINAL)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged FINAL)
    Q_PROPERTY(ElevationChart::Route route READ route WRITE setRoute NOTIFY routeChanged FINAL)

    public:
      explicit ChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] SG::BasicPalette palette() const; void setPalette(SG::BasicPalette);
      [[nodiscard]] bool intersecting() const;        void setIntersecting(bool);
      [[nodiscard]] bool valid() const;               void setValid(bool);
      [[nodiscard]] Route route() const;              void setRoute(const Route&);

    signals:
      void paletteChanged();
      void intersectingChanged();
      void validChanged();
      void routeChanged();

    protected:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;
      void requireRecolor();
      void fulfillRecolor();

    protected:
      bool m_require_recolor;
      QSGGeometryNode* m_background_node;

    private:
      SG::BasicPalette m_palette;
      bool m_intersecting;
      bool m_valid;
      Route m_route;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::ChartItem*);