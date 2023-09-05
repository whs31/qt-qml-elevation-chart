//
// Created by whs31 on 03.08.23.
//

#include "elevationchart.h"
#include <utility>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <QtConcurrent/QtConcurrent>
#include <LPVL/Utils>
#include <LPVL/Materials/FadingGradient>

#define in :

namespace ElevationChart
{
  enum OpenGLDrawMode
  {
    DrawQuadStrip = GL_QUAD_STRIP,
    DrawQuads = GL_QUADS
  };

  /**
   * \class ElevationChartItem
   * \brief Основной класс библиотеки.
   * \details Представляет собой C++ - реализацию для профиля высот.
   * Класс отвечает за вычисления, визуализацию с аппаратным ускорением и хранение данных о профиле высот.
   * \note Класс зарегистрирован как мета-тип и может использоваться в качестве Q_PROPERTY
   * по указателю как в C++, так и в QML.
   */

  /**
   * \brief Создает новый объект с указанным родителем. Этот конструктор должен быть вызван движком QML.
   * \param parent - родительский объект в иерархии Qt.
   */
  ElevationChartItem::ElevationChartItem(QQuickItem* parent)
    : LPVL::ScenegraphObject(parent)
    , m_researcher(new Researcher(this))
    , m_missing_tiles(false)
    , m_intersecting(false)
    , m_valid(false)
    , m_matching_metrics(true)
    , m_route(Route())
    , m_model(new RouteModel(this))
    , m_notifications(new NotificationModel(this))
    , m_uav_position(QGeoCoordinate(60, 30))
    , m_metrics(Metrics())
    , m_envelope(Envelope())
    , m_provider_type(ProviderType::DEMProvider)
    , m_bounds(Bounds())
    , m_shrink_mode(ShrinkMode::ShrinkToRouteHeight)
    , m_envelope_route(Route())
  {
    switch(providerType())
    {
      case ProviderType::RandomProvider: m_provider = std::make_unique<RandomDataProvider>(); break;
      case ProviderType::DEMProvider: m_provider = std::make_unique<DEMDataProvider>(); break;
      default: throw std::runtime_error("Weird shit happened, requiring investigation!");
    }

    this->setFlag(ItemHasContents);
    qRegisterMetaType<ElevationChartItem*>("ChartItem*");
    qRegisterMetaType<vector<ElevationPoint>>("vector<ElevationPoint>");

    connect(model(), &RouteModel::requireRebuild, this, [this](int index, float new_altitude){
      m_route.at(index).setAltitude(new_altitude);
      emit routeChanged();
      this->updateBounds();
    });

    connect(this, &ElevationChartItem::updateProfileFinished, this, &ElevationChartItem::receiveProfile);
    connect(researcher(), &Researcher::researchIntersectionsFinished, this, [this](const vector<ElevationPoint>& vec){
      m_intersections = vec;
      setIntersecting(not vec.empty());
      this->update();
    });

    connect(researcher(), &Researcher::researchEnvelopeFinished, this, [this](const Researcher::EnvelopeResult& res){
      m_envelope_route = res.route;
      m_envelopePathVec = m_envelope_route.toElevationGraph();
      m_envelopeCorridorVec = res.boundPolygon;
//      m_envelopePathVec.clear();
//      m_envelopeCorridorVec.clear();
//
//      for(const auto& p in res.routeProfile())
//        m_envelopePathVec.emplace_back(p.x(), p.y());
//
//      for(size_t i = 0; i < res.lowBound().size(); i++)
//      {
//        m_envelopeCorridorVec.emplace_back(res.lowBound()[i].x(), res.lowBound()[i].y());
//        m_envelopeCorridorVec.emplace_back(res.highBound()[i].x(), res.highBound()[i].y());
//      }

      this->update();
    });
  }

  /**
   * \brief Корректирует текущий путь в соответствии с ЛТХ борта.
   */
  [[maybe_unused]] void ElevationChartItem::applyMetricsCorrection() noexcept
  {
    if(matchingMetrics())
    {
      qWarning() << "<elevation-chart> Route already matching metrics, no correction will be applied";
      return;
    }

    this->setRoute(Route(m_metrics_path, metrics().fallbackVelocity()));
    qDebug() << "<elevation-chart> Route is corrected according to flight metrics";
  }

  [[maybe_unused]] void ElevationChartItem::estimateEnvelope() noexcept
  {
    qDebug() << "<elevation-chart> Envelope estimation requested";
    researcher()->researchEnvelope(route().toGeoPath(), metrics(), envelope());
  }

  [[maybe_unused]] void ElevationChartItem::applyEnvelopeCorrection() noexcept
  {
    qDebug() << "<elevation-chart> Envelope correction requested";
  }

  constexpr static float CORRIDOR_NODE_OPACITY = .3f;
  void ElevationChartItem::setupChildNodes(QSGNode* node)
  {
    QColor info2 = palette().info();
    info2.setAlphaF(CORRIDOR_NODE_OPACITY);

    tree()[BackgroundNode] = LPVL::utils::createSimpleGeometryNode(palette().background(), QSGGeometry::DrawTriangles, LPVL::utils::GeometryAndMaterial);
    tree()[ProfileNode] = LPVL::utils::createSimpleGeometryNode(palette().overlay(), OpenGLDrawMode::DrawQuadStrip, LPVL::utils::GeometryAndMaterial);
    tree()[RouteNode] = LPVL::utils::createSimpleGeometryNode(palette().accent(), QSGGeometry::DrawLineStrip, LPVL::utils::GeometryAndMaterial, ROUTE_LINE_WIDTH);
    tree()[MetricsNode] = LPVL::utils::createSimpleGeometryNode(palette().warn(), QSGGeometry::DrawLineStrip, LPVL::utils::GeometryAndMaterial, METRICS_LINE_WIDTH);
    tree()[MetricsPointNode] = LPVL::utils::createSimpleGeometryNode(palette().warn(), QSGGeometry::DrawPoints, LPVL::utils::GeometryAndMaterial, METRICS_ROUNDING_WIDTH);
    tree()[IntersectionsNode] = LPVL::utils::createSimpleGeometryNode(palette().error(), OpenGLDrawMode::DrawQuads, LPVL::utils::EmptyNode, METRICS_LINE_WIDTH);
    tree()[IntersectionsRouteNode] = LPVL::utils::createSimpleGeometryNode(palette().error(), QSGGeometry::DrawLines, LPVL::utils::GeometryAndMaterial, ROUTE_LINE_WIDTH);
    tree()[EnvelopeNode] = LPVL::utils::createSimpleGeometryNode(palette().info(), QSGGeometry::DrawLineStrip, LPVL::utils::GeometryAndMaterial, METRICS_LINE_WIDTH);
    tree()[EnvelopePointNode] = LPVL::utils::createSimpleGeometryNode(palette().info(), QSGGeometry::DrawPoints, LPVL::utils::GeometryAndMaterial, METRICS_ROUNDING_WIDTH);
    tree()[CorridorNode] = LPVL::utils::createSimpleGeometryNode(info2, OpenGLDrawMode::DrawQuadStrip, LPVL::utils::GeometryAndMaterial, METRICS_LINE_WIDTH);

    QSGSimpleMaterial<State>* material = LPVL::FadingGradientShader::createMaterial();
    material->setFlag(QSGMaterial::Blending);
    auto* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0, 0, QSGGeometry::UnsignedIntType);
    geometry->setDrawingMode(OpenGLDrawMode::DrawQuads);
    geometry->setLineWidth(METRICS_LINE_WIDTH);
    tree()[IntersectionsNode]->setMaterial(material);
    tree()[IntersectionsNode]->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
    tree()[IntersectionsNode]->setGeometry(geometry);

    for(const auto&[key, value] : tree())
      node->appendChildNode(value);
  }

  void ElevationChartItem::setupNodeColors(QSGNode* node)
  {
    QColor info2 = palette().info();
    info2.setAlphaF(CORRIDOR_NODE_OPACITY);

    dynamic_cast<QSGFlatColorMaterial*>(tree()[BackgroundNode]->material())->setColor(palette().background());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[ProfileNode]->material())->setColor(palette().overlay());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[RouteNode]->material())->setColor(palette().accent());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[MetricsNode]->material())->setColor(palette().warn());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[MetricsPointNode]->material())->setColor(palette().warn());
    dynamic_cast<QSGSimpleMaterial<State>*>(tree()[IntersectionsNode]->material())->state()->color = palette().error();
    dynamic_cast<QSGFlatColorMaterial*>(tree()[IntersectionsRouteNode]->material())->setColor(palette().error());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[EnvelopeNode]->material())->setColor(palette().info());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[EnvelopePointNode]->material())->setColor(palette().info());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[CorridorNode]->material())->setColor(info2);
  }

  void ElevationChartItem::drawCall(QSGNode* node)
  {
    this->handleBackgroundNode();
    this->handleProfileNode();
    this->handle(tree()[RouteNode]->geometry(), route().toElevationGraph());
    this->handleMetricsNode();
    this->handleIntersectionsNode();
    this->handle2nodes(tree()[EnvelopeNode]->geometry(), tree()[EnvelopePointNode]->geometry(), m_envelopePathVec, m_envelopePathVec.empty());
    this->handle(tree()[CorridorNode]->geometry(), m_envelopeCorridorVec, m_envelopeCorridorVec.empty());
  }

  void ElevationChartItem::updateProfile() noexcept
  {
    QFuture<void> outer = QtConcurrent::run([this](){
      QFuture<void> inner = QtConcurrent::run([this](){
        auto result = provider()->plotElevationProfile(route().toGeoPath());
        emit updateProfileFinished(result);
      });
    });
  }

  void ElevationChartItem::receiveProfile(const vector<ElevationPoint>& profile) noexcept
  {
    m_profile = profile;

    if(m_profile.empty() and route().valid())
      setMissingTiles(true);
    else
      setMissingTiles(false);

    this->updateBounds();

    auto t_route = route().toElevationGraph();
    model()->clear();
    for(const auto& point : t_route)
      model()->add(point);
  }

  void ElevationChartItem::updateBounds() noexcept
  {
    if(not route().valid())
      return;

    if(m_profile.empty())
      return;

    researcher()->researchIntersections(route().toGeoPath());

    setBounds({ m_profile.back().distance(), 0 });
    auto path = route().toGeoPath().path();
    for(const auto& coordinate in path)
      if(coordinate.altitude() > bounds().y())
        setBounds({bounds().x(), static_cast<float>(coordinate.altitude())});

    if(shrinkMode() == ShrinkToProfileHeight)
      setBounds({bounds().x(), std::max(bounds().y(), std::max_element(m_profile.cbegin(), m_profile.cend(),
                 [](const ElevationPoint& a, const ElevationPoint& b){ return a.elevation() < b.elevation();
                 })->elevation())});

    this->updateMetrics();
    this->update();
  }

  void ElevationChartItem::updateMetrics() noexcept
  {
    bool matching_metrics = true;

    QGeoPath correct_path;
    QGeoPath route_path = route().toGeoPath();
    if(route_path.isEmpty())
      return;

    auto velocities = route().velocities();

    correct_path.addCoordinate(route_path.path().first());
    for(int i = 1; i < route_path.path().size(); i++)
    {
      const bool fallback = velocities.at(i) == 0;
      const auto dx = static_cast<float>(route_path.path().at(i).distanceTo(route_path.path().at(i - 1)));
      const auto dy = static_cast<float>(route_path.path().at(i).altitude() - correct_path.path().at(i - 1).altitude());
      const float dy_min = fallback ? metrics().rateOfDescend() * dx / metrics().fallbackVelocity() : metrics().rateOfDescend() * dx / velocities.at(i);
      const float dy_max = fallback ? metrics().rateOfClimb() * dx / metrics().fallbackVelocity() : metrics().rateOfClimb() * dx / velocities.at(i);

      auto result = route_path.path().at(i);

      // correct case
      if((dy >= 0 and dy < dy_max) or (dy <= 0 and abs(dy) < dy_min))
      {
        correct_path.addCoordinate(result);
        continue;
      }

      // climbs too fast
      else if(dy > 0 and dy > dy_max)
      {
        result.setAltitude(correct_path.path().at(i - 1).altitude() + dy_max);
        matching_metrics = false;
      }

      // descends too fast
      else if(dy < 0 and abs(dy) > dy_min)
      {
        result.setAltitude(correct_path.path().at(i - 1).altitude() - dy_min);
        matching_metrics = false;
      }

      correct_path.addCoordinate(result);
    }

    if(not matching_metrics)
    {
      setMatchingMetrics(false);
      m_metrics_path = correct_path;
    }
    else
      setMatchingMetrics(true);
  }

  void ElevationChartItem::handleBackgroundNode() noexcept
  {
    auto geometry = tree()[BackgroundNode]->geometry();

    geometry->allocate(6);
    geometry->vertexDataAsPoint2D()[0].set(0, 0);
    geometry->vertexDataAsPoint2D()[1].set(static_cast<float>(width()), 0);
    geometry->vertexDataAsPoint2D()[2].set(static_cast<float>(width()), static_cast<float>(height()));
    geometry->vertexDataAsPoint2D()[3].set(static_cast<float>(width()), static_cast<float>(height()));
    geometry->vertexDataAsPoint2D()[4].set(0, static_cast<float>(height()));
    geometry->vertexDataAsPoint2D()[5].set(0, 0);
  }

  void ElevationChartItem::handleProfileNode() noexcept
  {
    auto geometry = tree()[ProfileNode]->geometry();

    vector<QSGGeometry::Point2D> gl;
    for(const auto& point in m_profile)
    {
      gl.push_back({toPixelX(point.distance(), bounds().x()), static_cast<float>(height())});
      gl.push_back(toPixel(point.distance(), point.elevation()));
    }

    geometry->allocate(static_cast<int>(gl.size()));

    for(size_t i = 0; i < gl.size(); i++)
      geometry->vertexDataAsPoint2D()[i] = gl.at(i);
  }

  void ElevationChartItem::handleMetricsNode() noexcept
  {
    auto geometry1 = tree()[MetricsNode]->geometry();
    auto geometry2 = tree()[MetricsPointNode]->geometry();

    if(matchingMetrics())
    {
      /*
       * Окей, это не совсем очевидная херня, поэтому я прокомментирую будущим поколениям.
       * Если мы не будем очищать буфер ноды, то при отсутствии вызова обновления метрик в
       * соответствии с маршрутом мы получим невалидные данные. Если эта нода была бы отдельным
       * компонентом в qml, все решилось бы visible: !matchingMetrics, но внутри одного item-а
       * единственный варик это вручную чистить буфер вот таким вызовом.
       */
      geometry1->allocate(0);
      geometry2->allocate(0);
      return;
    }

    vector<ElevationPoint> t_route;
    auto prev_coord = m_metrics_path.path().front();
    float distance = 0;
    for(const auto& point in m_metrics_path.path())
    {
      distance += static_cast<float>(point.distanceTo(prev_coord));
      t_route.emplace_back(distance, point.altitude());
      prev_coord = point;
    }

    vector<QSGGeometry::Point2D> gl;
    for(const auto& point in t_route)
      gl.push_back(toPixel(point.distance(), point.elevation()));

    geometry1->allocate(static_cast<int>(gl.size()));
    geometry2->allocate(static_cast<int>(gl.size()));
    for(size_t i = 0; i < gl.size(); i++)
    {
      geometry1->vertexDataAsPoint2D()[i] = gl.at(i);
      geometry2->vertexDataAsPoint2D()[i] = gl.at(i);
    }
  }

  void ElevationChartItem::handleIntersectionsNode() noexcept
  {
    if(not intersecting())
    {
      tree()[IntersectionsNode]->geometry()->allocate(0);
      tree()[IntersectionsRouteNode]->geometry()->allocate(0);
      return;
    }

    auto geometry = tree()[IntersectionsNode]->geometry();
    auto geometry2 = tree()[IntersectionsRouteNode]->geometry();

    vector<QSGGeometry::TexturedPoint2D> gl;
    vector<QSGGeometry::Point2D> gl_x;
    bool flip = false;
    for(const auto& point in m_intersections)
    {
      gl_x.push_back(toPixel(point.distance(), point.elevation()));
      if(flip)
      {
        gl.push_back(LPVL::utils::fromPoint2DBounded(toPixel(point.distance(), point.elevation()), width(), height()));
        gl.push_back(LPVL::utils::fromPoint2DBounded({toPixelX(point.distance(), bounds().x()), static_cast<float>(height())}, width(), height()));
        flip ^= 1;
      }
      else
      {
        gl.push_back(LPVL::utils::fromPoint2DBounded({toPixelX(point.distance(), bounds().x()), static_cast<float>(height())}, width(), height()));
        gl.push_back(LPVL::utils::fromPoint2DBounded(toPixel(point.distance(), point.elevation()), width(), height()));
        flip ^= 1;
      }
    }

    if(gl.size() % 2 != 0)
      gl.push_back(LPVL::utils::fromPoint2DBounded({toPixelX(m_intersections.back().distance(), bounds().x()), static_cast<float>(height())}, width(), height()));

    geometry->allocate(static_cast<int>(gl.size()));
    geometry2->allocate(static_cast<int>(gl_x.size()));
    for(size_t i = 0; i < gl.size(); i++)
      geometry->vertexDataAsTexturedPoint2D()[i] = gl.at(i);

    for(size_t i = 0; i < gl_x.size(); i++)
      geometry2->vertexDataAsPoint2D()[i] = gl_x.at(i);
  }

  void ElevationChartItem::handle(QSGGeometry* geometry, const vector<ElevationPoint>& vec, bool abort_condition)
  {
    if(abort_condition)
    {
      geometry->allocate(0);
      return;
    }

    vector<QSGGeometry::Point2D> gl;
    for(const auto& point in vec)
      gl.push_back(toPixel(point));
    geometry->allocate(static_cast<int>(gl.size()));
    for(size_t i = 0; i < gl.size(); i++)
      geometry->vertexDataAsPoint2D()[i] = gl[i];
  }

  void ElevationChartItem::handle2nodes(QSGGeometry* geometry1, QSGGeometry* geometry2, const vector<ElevationPoint>& vec, bool abort_condition)
  {
    if(abort_condition)
    {
      geometry1->allocate(0);
      geometry2->allocate(0);
      return;
    }

    vector<QSGGeometry::Point2D> gl;
    for(const auto& point in vec)
      gl.push_back(toPixel(point));
    geometry1->allocate(static_cast<int>(gl.size()));
    geometry2->allocate(static_cast<int>(gl.size()));
    for(size_t i = 0; i < gl.size(); i++)
    {
      geometry1->vertexDataAsPoint2D()[i] = gl[i];
      geometry2->vertexDataAsPoint2D()[i] = gl[i];
    }
  }

  /**
   * \brief Преобразует координаты ElevationPoint в пиксельные координаты этого объекта.
   * \param x - значение ElevationPoint::distance.
   * \param y - значение ElevationPoint::elevation.
   * \param b - набор максимальных значений. Если не задан (по умолчанию), то будет использован набор этого экземпляра класса.
   * \return Точка для использования в цикле отрисовки.
   */
  QSGGeometry::Point2D ElevationChartItem::toPixel(float x, float y, Bounds b) const { return {toPixelX(x, b.x() > 0 ? b.x()
    : bounds().x()), toPixelY(y, b.y() > 0 ? b.y() : bounds().y()) }; }
  QSGGeometry::Point2D ElevationChartItem::toPixel(ElevationPoint p, Bounds b) const { return {toPixelX(p.distance(), b.x() > 0 ? b.x()
    : bounds().x()), toPixelY(p.elevation(), b.y() > 0 ? b.y() : bounds().y()) }; }

  /**
   * \brief Вспомогательная функция, преобразующая координаты ElevationPoint в пиксельные только по оси X.
   * \param x - значение ElevationPoint::distance.
   * \param x_max - максимальное значение по оси X.
   * \return Значение по оси Х в пикселях.
   */
  float ElevationChartItem::toPixelX(float x, float x_max) const { return static_cast<float>(x / x_max * width()); }

  /**
   * \brief Вспомогательная функция, преобразующая координаты ElevationPoint в пиксельные только по оси Y.
   * \param y - значение ElevationPoint::elevation.
   * \param y_max - максимальное значение по оси Y.
   * \return Значение по оси Y в пикселях.
   */
  float ElevationChartItem::toPixelY(float y, float y_max) const { return static_cast<float>(height() - (y / (y_max * bounds().stretch()) * height())); }
} // ElevationChart