//
// Created by whs31 on 03.08.23.
//

#include "elevationchart.h"
#include <utility>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <SG/Utils>

namespace ElevationChart
{
  constexpr static const unsigned int DrawQuadStrip = GL_QUAD_STRIP;

  /**
   * \class ElevationChartItem
   * \brief Основной класс библиотеки.
   * \details Представляет собой C++ - реализацию для профиля высот.
   * Класс отвечает за вычисления, отрисовку с аппаратным ускорением и хранение данных о профиле высот.
   * \note Класс зарегистрирован как мета-тип и может использоваться в качестве Q_PROPERTY
   * по указателю как в C++, так и в QML.
   */

  /**
   * \brief Создает новый объект с указанным родителем. Этот конструктор должен быть вызван движком QML.
   * \param parent - родительский объект в иерархии Qt.
   */
  ElevationChartItem::ElevationChartItem(QQuickItem* parent)
    : SG::ScenegraphObject(parent)
    , m_intersecting(false)
    , m_valid(false)
    , m_matching_metrics(true)
    , m_route(Route())
    , m_model(new RouteModel(this))
    , m_uav_position(QGeoCoordinate(60, 30))
    , m_metrics(Metrics())
    , m_random_provider(std::make_unique<RandomDataProvider>())
    , m_bounds(Bounds())
    , m_shrink_mode(ShrinkMode::ShrinkToRouteHeight)
  {
    this->setFlag(ItemHasContents);
    qRegisterMetaType<ElevationChartItem*>("ChartItem*");

    connect(model(), &RouteModel::requireRebuild, this, [this](int index, float new_altitude){
      m_route.at(index).setAltitude(new_altitude);
      emit routeChanged();
      this->updateBounds();
    });
  }

  /**
   * \brief Корректирует текущий путь в соответствии с ЛТХ борта.
   */
  void ElevationChartItem::applyMetricsCorrection() noexcept
  {
    if(matchingMetrics())
    {
      qWarning() << "<elevation-chart> Route already matching metrics, no correction will be applied";
      return;
    }

    this->setRoute(Route(m_metrics_path, metrics().fallbackVelocity()));
    qDebug() << "<elevation-chart> Route is corrected according to flight metrics.";
  }

  void ElevationChartItem::setupChildNodes(QSGNode* node)
  {
    tree()[BackgroundNode] = SG::utils::createSimpleGeometryNode(palette().background(), QSGGeometry::DrawTriangles);
    tree()[ProfileNode] = SG::utils::createSimpleGeometryNode(palette().overlay(), DrawQuadStrip);
    tree()[RouteNode] = SG::utils::createSimpleGeometryNode(palette().accent(), QSGGeometry::DrawLineStrip, ROUTE_LINE_WIDTH);
    tree()[MetricsNode] = SG::utils::createSimpleGeometryNode(palette().warn(), QSGGeometry::DrawLineStrip, METRICS_LINE_WIDTH);
    tree()[MetricsPointNode] = SG::utils::createSimpleGeometryNode(palette().warn(), QSGGeometry::DrawPoints, METRICS_ROUNDING_WIDTH);

    for(const auto&[key, value] : tree())
      node->appendChildNode(value);
  }

  void ElevationChartItem::setupNodeColors(QSGNode* node)
  {
    dynamic_cast<QSGFlatColorMaterial*>(tree()[BackgroundNode]->material())->setColor(palette().background());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[ProfileNode]->material())->setColor(palette().overlay());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[RouteNode]->material())->setColor(palette().accent());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[MetricsNode]->material())->setColor(palette().warn());
    dynamic_cast<QSGFlatColorMaterial*>(tree()[MetricsPointNode]->material())->setColor(palette().warn());
  }

  void ElevationChartItem::drawCall(QSGNode* node)
  {
    this->handleBackgroundNode();
    this->handleProfileNode();
    this->handleRouteNode();
    this->handleMetricsNode();
  }

  void ElevationChartItem::updateProfile() noexcept
  {
    m_profile = m_random_provider->plotElevationProfile(route().toGeoPath());

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

    setBounds({ m_profile.back().distance(), 0 });
    auto path = route().toGeoPath().path();
    for(const auto& coordinate : path)
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

    //bool allow_individual_speeds = not qFuzzyCompare(route().at(0).velocity(), 0);
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
    for(const auto& point : m_profile)
    {
      gl.push_back({toPixelX(point.distance(), bounds().x()), static_cast<float>(height())});
      gl.push_back(toPixel(point.distance(), point.elevation(), bounds().x(), bounds().y()));
    }

    geometry->allocate(static_cast<int>(gl.size()));

    for(size_t i = 0; i < gl.size(); i++)
      geometry->vertexDataAsPoint2D()[i] = gl.at(i);
  }

  void ElevationChartItem::handleRouteNode() noexcept
  {
    auto geometry = tree()[RouteNode]->geometry();

    vector<ElevationPoint> t_route = route().toElevationGraph();
    vector<QSGGeometry::Point2D> gl;

    for(const auto& point : t_route)
      gl.push_back(toPixel(point.distance(), point.elevation(), bounds().x(), bounds().y()));

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
       * окей, это не совсем очевидная херня, поэтому я прокомментирую будущим поколениям.
       * если мы не будем очищать буфер ноды, то при отсутствии вызова обновления метрик в
       * соответствии с маршрутом мы получим невалидные данные. если эта нода была бы отдельным
       * компонентом в qml, все решилось бы visible: !matchingMetrics, но внутри одного итема
       * единственный варик это вручную чистить буфер вот таким вызовом.
       */
      geometry1->allocate(0);
      geometry2->allocate(0);
      return;
    }

    vector<ElevationPoint> t_route;
    auto prev_coord = m_metrics_path.path().front();
    float distance = 0;
    for(const auto& point : m_metrics_path.path())
    {
      distance += static_cast<float>(point.distanceTo(prev_coord));
      t_route.emplace_back(distance, point.altitude());
      prev_coord = point;
    }

    vector<QSGGeometry::Point2D> gl;

    for(const auto& point : t_route)
      gl.push_back(toPixel(point.distance(), point.elevation(), bounds().x(), bounds().y()));

    geometry1->allocate(static_cast<int>(gl.size()));
    geometry2->allocate(static_cast<int>(gl.size()));
    for(size_t i = 0; i < gl.size(); i++)
    {
      geometry1->vertexDataAsPoint2D()[i] = gl.at(i);
      geometry2->vertexDataAsPoint2D()[i] = gl.at(i);
    }
  }

  /**
   * \brief Преобразует координаты ElevationPoint в пиксельные координаты этого объекта.
   * \param x - значение ElevationPoint::distance.
   * \param y - значение ElevationPoint::elevation.
   * \param x_max - максимальное значение по оси X.
   * \param y_max - максимальное значение по оси Y.
   * \return Точка для использования в цикле отрисовки.
   */
  QSGGeometry::Point2D ElevationChartItem::toPixel(float x, float y, float x_max, float y_max) const { return {toPixelX(x, x_max), toPixelY(y, y_max) }; }

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

  // properties

  /**
   * \property ElevationChartItem::palette
   * \brief Палитра цветов, используемая в виджете.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>palette</i>     <td><i>setPalette</i>   <td><i>paletteChanged</i>
   * </table>
   * \see SG::BasicPalette
   */
  SG::BasicPalette ElevationChartItem::palette() const { return m_palette; }
  void ElevationChartItem::setPalette(SG::BasicPalette x) {
    m_palette = std::move(x);
    emit paletteChanged();

    this->requireRecolor();
  }

  /**
   * \property ElevationChartItem::bounds
   * \brief Максимальные значения по осям для виджета.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>bounds</i>      <td><i>setBounds</i>    <td><i>boundsChanged</i>
   * </table>
   * \see ElevationChart::Bounds
   * \warning Запись в свойство из QML приведет к неопределенному поведению.
   */
  Bounds ElevationChartItem::bounds() const { return m_bounds; }
  void ElevationChartItem::setBounds(ElevationChart::Bounds x) {
    m_bounds = x;
    emit boundsChanged();
  }

  /**
   * \property ElevationChartItem::intersecting
   * \brief Состояние пересечения пути с рельефом.
   * \details
   * Свойство вернет <tt>true</tt>, если путь пересекается с рельефом хотя бы в одной точке.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись                <th>Оповещение
   * <tr><td><i>intersecting</i><td><i>setIntersecting</i><td><i>intersectingChanged</i>
   * </table>
   */
  bool ElevationChartItem::intersecting() const { return m_intersecting; }
  void ElevationChartItem::setIntersecting(bool x) {
    if(x == m_intersecting)
      return;
    m_intersecting = x;
    emit intersectingChanged();
  }

  /**
   * \property ElevationChartItem::valid
   * \brief Состояние наличия профилей высот.
   * \details
   * Свойство вернет <tt>false</tt>, если отсутствуют профили высот для выбранного пути.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись                <th>Оповещение
   * <tr><td><i>valid</i>       <td><i>setValid</i>       <td><i>validChanged</i>
   * </table>
   */
  bool ElevationChartItem::valid() const { return m_valid; }
  void ElevationChartItem::setValid(bool x) {
    if(x == m_valid)
      return;
    m_valid = x;
    emit validChanged();
  }

  /**
   * \property ElevationChartItem::matchingMetrics
   * \brief Состояние соответствия пути ЛТХ борта.
   * \details
   * Свойство вернет <tt>false</tt>, если текущий заданный путь не соответствует
   * летно-техническим характеристикам БПЛА.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение                 <th>Запись                    <th>Оповещение
   * <tr><td><i>matchingMetrics</i> <td><i>setMatchingMetrics</i> <td><i>matchingMetricsChanged</i>
   * </table>
   */
  bool ElevationChartItem::matchingMetrics() const { return m_matching_metrics; }
  void ElevationChartItem::setMatchingMetrics(bool x) {
    if(x == m_matching_metrics)
      return;
    m_matching_metrics = x;
    emit matchingMetricsChanged();
  }

  /**
   * \property ElevationChartItem::route
   * \brief Путь, по которому происходит построения профиля высот.
   * \details
   * Это основное свойство, используемое извне. При обновлении пути виджет перестраивает
   * текущий профиль, а при изменении пути из виджета срабатывает сигнал, и путь можно прочитать
   * обратно.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>route</i>       <td><i>setRoute</i>     <td><i>routeChanged</i>
   * </table>
   * \see ElevationChart::Route
   */
  Route ElevationChartItem::route() const { return m_route; }
  void ElevationChartItem::setRoute(const Route& x) {
    m_route = x;
    emit routeChanged();

    this->updateProfile();
  }

  /**
   * \property ElevationChartItem::model
   * \brief Модель точек пути.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>model</i>       <td><i>--</i>           <td><i>--</i>
   * </table>
   * \see ElevationChart::RouteModel
   */
  RouteModel* ElevationChartItem::model() const { return m_model; }

  /**
   * \property ElevationChartItem::uavPosition
   * \brief Позиция БПЛА в данный момент.
   * \details
   * Используется для вычисления относительной высоты и отображении позиции БПЛА
   * на виджете.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись               <th>Оповещение
   * <tr><td><i>uavPosition</i> <td><i>setUavPosition</i><td><i>uavPositionChanged</i>
   * </table>
   */
  QGeoCoordinate ElevationChartItem::uavPosition() const { return m_uav_position; }
  void ElevationChartItem::setUavPosition(const QGeoCoordinate& x) {
    if(x == m_uav_position)
      return;
    m_uav_position = x;
    emit uavPositionChanged();
  }

  /**
   * \property ElevationChartItem::metrics
   * \brief Группа параметров, связанных с коррекцией по ЛТХ.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись               <th>Оповещение
   * <tr><td><i>metrics</i>     <td><i>setMetrics</i>    <td><i>metricsChanged</i>
   * </table>
   */
  Metrics ElevationChartItem::metrics() const { return m_metrics; }
  void ElevationChartItem::setMetrics(const Metrics& x) {
    m_metrics = x;
    emit metricsChanged();

    this->updateMetrics();
    this->update();
  }

  /**
   * \property ElevationChartItem::shrinkMode
   * \brief Тип поведения оси Y виджета.
   * \details
   * <i>По умолчанию равняется ShrinkMode::ShrinkToRouteHeight</i>.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>shrinkMode</i>  <td><i>setShrinkMode</i><td><i>shrinkModeChanged</i>
   * </table>
   * \see ShrinkMode
   */
  int ElevationChartItem::shrinkMode() const { return static_cast<int>(m_shrink_mode); }
  void ElevationChartItem::setShrinkMode(int x) {
    if(x == m_shrink_mode)
      return;
    m_shrink_mode = static_cast<ShrinkMode>(x);
    emit shrinkModeChanged();

    this->updateBounds();
  }

  map<ElevationChartItem::NodeTypes, QSGGeometryNode*>& ElevationChartItem::tree() { return m_tree; }
} // ElevationChart