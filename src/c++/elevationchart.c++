//
// Created by whs31 on 03.08.23.
//

#include "elevationchart.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <utility>

namespace ElevationChart
{
  ChartItem::ChartItem(QQuickItem* parent)
    : QQuickItem(parent)
    , m_require_recolor(false)
    , m_background_node(nullptr) // !
    , m_profile_node(nullptr) // !
    , m_intersecting(false)
    , m_valid(false)
    , m_route(Route())
    , m_uav_position(QGeoCoordinate(60, 30))
    , m_random_provider(std::make_unique<RandomDataProvider>())
    , m_bound({0, 0})
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

      m_profile_node = new QSGGeometryNode;
      auto* profile_material = new QSGFlatColorMaterial;
      profile_material->setColor(palette().overlay());
      m_profile_node->setMaterial(profile_material);
      auto* profile_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0, QSGGeometry::UnsignedIntType);
      profile_geometry->setDrawingMode(GL_QUAD_STRIP);
      m_profile_node->setGeometry(profile_geometry);
      m_profile_node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);

      old_node->appendChildNode(m_background_node);
      old_node->appendChildNode(m_profile_node);
    }

    if(m_require_recolor)
    {
      dynamic_cast<QSGFlatColorMaterial*>(m_background_node->material())->setColor(palette().background());
      dynamic_cast<QSGFlatColorMaterial*>(m_profile_node->material())->setColor(palette().overlay());
    }

    m_background_node->geometry()->allocate(6);
    m_background_node->geometry()->vertexDataAsPoint2D()[0].set(0, 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[1].set(static_cast<float>(width()), 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[2].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[3].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[4].set(0, static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[5].set(0, 0);

    vector<QSGGeometry::Point2D> profile_gl;
    for(const auto& point : m_profile)
    {
      profile_gl.push_back({static_cast<float>(point.distance() / m_bound.x_max * width()),
                               static_cast<float>(height())});
      profile_gl.push_back({static_cast<float>(point.distance() / m_bound.x_max * width()),
                            static_cast<float>(height() - (point.elevation() / m_bound.y_max * height()))});
    }

    m_profile_node->geometry()->allocate(static_cast<int>(profile_gl.size()));
    for(size_t i = 0; i < profile_gl.size(); i++)
      m_profile_node->geometry()->vertexDataAsPoint2D()[i] = profile_gl.at(i);

    if(m_require_recolor)
    {
      m_background_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
      m_profile_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
      this->fulfillRecolor();
    }
    else
    {
      m_background_node->markDirty(QSGNode::DirtyGeometry);
      m_profile_node->markDirty(QSGNode::DirtyGeometry);
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

  void ChartItem::calculate() noexcept
  {
    m_profile = m_random_provider->plotElevationProfile(m_route.toGeoPath());
    m_bound = { m_profile.back().distance(), std::max_element(m_profile.cbegin(), m_profile.cend(),
                [](const ElevationPoint& a, const ElevationPoint& b){
                  return a.elevation() < b.elevation(); })->elevation() };
    qDebug() << m_bound.x_max << m_bound.y_max;
  }

  // properties

  SG::BasicPalette ChartItem::palette() const { return m_palette; }
  void ChartItem::setPalette(SG::BasicPalette x) {
    m_palette = std::move(x);
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

    this->calculate();
  }

  QGeoCoordinate ChartItem::uavPosition() const { return m_uav_position; }
  void ChartItem::setUavPosition(const QGeoCoordinate& x) {
    if(x == m_uav_position)
      return;
    m_uav_position = x;
    emit uavPositionChanged();
  }
} // ElevationChart