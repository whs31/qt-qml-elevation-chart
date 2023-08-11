//
// Created by whs31 on 03.08.23.
//

#include "elevationchart.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <utility>

namespace ElevationChart
{
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
    : QQuickItem(parent)
    , m_require_recolor(false)
    , m_background_node(nullptr)
    , m_profile_node(nullptr)
    , m_route_node(nullptr)
    , m_intersecting(false)
    , m_valid(false)
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

  QSGNode* ElevationChartItem::updatePaintNode(QSGNode* old_node, QQuickItem::UpdatePaintNodeData* unused)
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

  void ElevationChartItem::requireRecolor()
  {
    m_require_recolor = true;
    this->update();
  }

  void ElevationChartItem::fulfillRecolor() { m_require_recolor = false; }

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
                 [](const ElevationPoint& a, const ElevationPoint& b){  return a.elevation() < b.elevation();
                 })->elevation())});

    this->update();
  }

  void ElevationChartItem::handleBackgroundNode() noexcept
  {
    m_background_node->geometry()->allocate(6);
    m_background_node->geometry()->vertexDataAsPoint2D()[0].set(0, 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[1].set(static_cast<float>(width()), 0);
    m_background_node->geometry()->vertexDataAsPoint2D()[2].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[3].set(static_cast<float>(width()), static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[4].set(0, static_cast<float>(height()));
    m_background_node->geometry()->vertexDataAsPoint2D()[5].set(0, 0);
  }

  void ElevationChartItem::handleProfileNode() noexcept
  {
    vector<QSGGeometry::Point2D> gl;
    for(const auto& point : m_profile)
    {
      gl.push_back({toPixelX(point.distance(), bounds().x()), static_cast<float>(height())});
      gl.push_back(toPixel(point.distance(), point.elevation(), bounds().x(), bounds().y()));
    }

    m_profile_node->geometry()->allocate(static_cast<int>(gl.size()));

    for(size_t i = 0; i < gl.size(); i++)
      m_profile_node->geometry()->vertexDataAsPoint2D()[i] = gl.at(i);
  }

  void ElevationChartItem::handleRouteNode() noexcept
  {
    vector<ElevationPoint> t_route = route().toElevationGraph();
    vector<QSGGeometry::Point2D> gl;

    for(const auto& point : t_route)
      gl.push_back(toPixel(point.distance(), point.elevation(), bounds().x(), bounds().y()));

    m_route_node->geometry()->allocate(static_cast<int>(gl.size()));
    for(size_t i = 0; i < gl.size(); i++)
      m_route_node->geometry()->vertexDataAsPoint2D()[i] = gl.at(i);
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
} // ElevationChart