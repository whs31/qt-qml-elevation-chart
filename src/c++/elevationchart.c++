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
    , m_background_node(nullptr)
    , m_profile_node(nullptr)
    , m_route_node(nullptr)
    , m_intersecting(false)
    , m_valid(false)
    , m_route(Route())
    , m_uav_position(QGeoCoordinate(60, 30))
    , m_random_provider(std::make_unique<RandomDataProvider>())
    , m_bound({0, 0})
    , m_shrink_mode(ShrinkMode::ShrinkToRouteHeight)
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

      m_route_node = new QSGGeometryNode;
      auto* route_material = new QSGFlatColorMaterial;
      route_material->setColor(palette().accent());
      m_route_node->setMaterial(route_material);
      auto* route_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0, QSGGeometry::UnsignedIntType);
      route_geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
      route_geometry->setLineWidth(ROUTE_LINE_WIDTH);
      m_route_node->setGeometry(route_geometry);
      m_route_node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);

      old_node->appendChildNode(m_background_node);
      old_node->appendChildNode(m_profile_node);
      old_node->appendChildNode(m_route_node);
    }

    if(m_require_recolor)
    {
      dynamic_cast<QSGFlatColorMaterial*>(m_background_node->material())->setColor(palette().background());
      dynamic_cast<QSGFlatColorMaterial*>(m_profile_node->material())->setColor(palette().overlay());
      dynamic_cast<QSGFlatColorMaterial*>(m_route_node->material())->setColor(palette().accent());
    }

    this->handleBackgroundNode();
    this->handleProfileNode();
    this->handleRouteNode();

    if(m_require_recolor)
    {
      m_background_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
      m_profile_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
      m_route_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
      this->fulfillRecolor();
    }
    else
    {
      m_background_node->markDirty(QSGNode::DirtyGeometry);
      m_profile_node->markDirty(QSGNode::DirtyGeometry);
      m_route_node->markDirty(QSGNode::DirtyGeometry);
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
  ChartItem::Bound& ChartItem::bounds() { return m_bound; }

  void ChartItem::updateProfile() noexcept
  {
    m_profile = m_random_provider->plotElevationProfile(m_route.toGeoPath());
    this->updateBounds();
  }

  void ChartItem::updateBounds() noexcept
  {
    if(not route().valid())
      return;

    bounds() = { m_profile.back().distance(), 0 };
    auto path = route().toGeoPath().path();
    for(const auto& coordinate : path)
      if(coordinate.altitude() > bounds().y_max)
        bounds().y_max = static_cast<float>(coordinate.altitude());

    if(shrinkMode() == ShrinkToProfileHeight)
      bounds().y_max = std::max(bounds().y_max, std::max_element(m_profile.cbegin(), m_profile.cend(),
                               [](const ElevationPoint& a, const ElevationPoint& b){  return a.elevation() < b.elevation();
                               })->elevation());

    this->update();
  }

  void ChartItem::handleBackgroundNode() noexcept
  {
    m_background_node->geometry()->allocate(6);
    m_background_node->geometry()->vertexDataAsPoint2D()[0].set(0, 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[1].set(static_cast<float>(width()), 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[2].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[3].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[4].set(0, static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[5].set(0, 0);
  }

  void ChartItem::handleProfileNode() noexcept
  {
    vector<QSGGeometry::Point2D> gl;
    for(const auto& point : m_profile)
    {
      gl.push_back({toPixelX(point.distance(), bounds().x_max), static_cast<float>(height())});
      gl.push_back(toPixel(point.distance(), point.elevation(), bounds().x_max, bounds().y_max));
    }

    m_profile_node->geometry()->allocate(static_cast<int>(gl.size()));

    for(size_t i = 0; i < gl.size(); i++)
      m_profile_node->geometry()->vertexDataAsPoint2D()[i] = gl.at(i);
  }

  void ChartItem::handleRouteNode() noexcept
  {
    vector<ElevationPoint> t_route = route().toElevationGraph();
    vector<QSGGeometry::Point2D> gl;

    for(const auto& point : t_route)
      gl.push_back(toPixel(point.distance(), point.elevation(), bounds().x_max, bounds().y_max));

    m_route_node->geometry()->allocate(static_cast<int>(gl.size()));
    for(size_t i = 0; i < gl.size(); i++)
      m_route_node->geometry()->vertexDataAsPoint2D()[i] = gl.at(i);
  }

  QSGGeometry::Point2D ChartItem::toPixel(float x, float y, float x_max, float y_max) const { return { toPixelX(x, x_max), toPixelY(y, y_max) }; }
  float ChartItem::toPixelX(float x, float x_max) const { return static_cast<float>(x / x_max * width()); }
  float ChartItem::toPixelY(float y, float y_max) const { return static_cast<float>(height() - (y / (y_max * STRETCH) * height())); }

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

    this->updateProfile();
  }

  QGeoCoordinate ChartItem::uavPosition() const { return m_uav_position; }
  void ChartItem::setUavPosition(const QGeoCoordinate& x) {
    if(x == m_uav_position)
      return;
    m_uav_position = x;
    emit uavPositionChanged();
  }

  int ChartItem::shrinkMode() const { return static_cast<int>(m_shrink_mode); }
  void ChartItem::setShrinkMode(int x) {
    if(x == m_shrink_mode)
      return;
    m_shrink_mode = static_cast<ShrinkMode>(x);
    emit shrinkModeChanged();

    this->updateBounds();
  }
} // ElevationChart