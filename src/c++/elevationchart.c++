//
// Created by whs31 on 03.08.23.
//

#include "elevationchart.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

namespace ElevationChart
{
  ChartItem::ChartItem(QQuickItem* parent)
    : QQuickItem(parent)
    , m_require_recolor(false)
    , m_background_node(nullptr) // !
    , m_intersecting(false)
    , m_valid(false)
    , m_route(Route())
  {
    this->setFlag(ItemHasContents);
    qRegisterMetaType<ChartItem*>("ChartItem*");
  }

  QSGNode* ChartItem::updatePaintNode(QSGNode* old_node, QQuickItem::UpdatePaintNodeData* unused)
  {
    (void)unused;

    if(old_node == nullptr)
    {
      old_node = new QSGNode;

      m_background_node = new QSGGeometryNode;
      auto* background_material = new QSGFlatColorMaterial;
      background_material->setColor(palette().background());
      m_background_node->setMaterial(background_material);
      auto* background_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0, QSGGeometry::UnsignedIntType);
      background_geometry->setDrawingMode(QSGGeometry::DrawTriangles);
      m_background_node->setGeometry(background_geometry);
      m_background_node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);

      old_node->appendChildNode(m_background_node);
    }

    if(m_require_recolor)
    {
      dynamic_cast<QSGFlatColorMaterial*>(m_background_node->material())->setColor(palette().background());
    }

    m_background_node->geometry()->allocate(6);
    m_background_node->geometry()->vertexDataAsPoint2D()[0].set(0, 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[1].set(static_cast<float>(width()), 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[2].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[3].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[4].set(0, static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[5].set(0, 0);

    if(m_require_recolor)
    {
      m_background_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
      this->fulfillRecolor();
    }
    else
    {
      m_background_node->markDirty(QSGNode::DirtyGeometry);
    }

    old_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return old_node;
  }

  void ChartItem::requireRecolor()
  {
    m_require_recolor = true;
    this->update();
  }

  void ChartItem::fulfillRecolor() { m_require_recolor = false; }

  SG::BasicPalette ChartItem::palette() const { return m_palette; }
  void ChartItem::setPalette(SG::BasicPalette x) {
    m_palette = x;
    emit paletteChanged();

    this->requireRecolor();
  }

  bool ChartItem::intersecting() const { return m_intersecting; }
  void ChartItem::setIntersecting(bool x) {
    if(x == m_intersecting)
      return;
    m_intersecting = x;
    emit intersectingChanged();
  }

  bool ChartItem::valid() const { return m_valid; }
  void ChartItem::setValid(bool x) {
    if(x == m_valid)
      return;
    m_valid = x;
    emit validChanged();
  }

  Route ChartItem::route() const { return m_route; }
  void ChartItem::setRoute(const Route& x) {
    m_route = x;
    emit routeChanged();
  }
} // ElevationChart