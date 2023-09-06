//
// Created by whs31 on 22.08.23.
//

#include "elevationchart.h"

namespace ElevationChart
{
  /**
   * \property ElevationChartItem::palette
   * \brief Палитра цветов, которую использует виджет.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>palette</i>     <td><i>setPalette</i>   <td><i>paletteChanged</i>
   * </table>
   * \see LPVL::BasicPalette
   */
  Palette ElevationChartItem::palette() const { return m_palette; }
  void ElevationChartItem::setPalette(Palette x) {
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
   * \property ElevationChartItem::missingTiles
   * \brief Состояние наличия необходимых профилей в папке с профилями.
   * \details
   * Свойство вернет <tt>true</tt>, если для заданного пути отсутствуют нужные профили.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись                <th>Оповещение
   * <tr><td><i>missingTiles</i><td><i>setMissingTiles</i><td><i>missingTilesChanged</i>
   * </table>
   */
  bool ElevationChartItem::missingTiles() const { return m_missing_tiles; }
  void ElevationChartItem::setMissingTiles(bool x) {
    if(x == m_missing_tiles)
      return;
    m_missing_tiles = x;
    emit missingTilesChanged();

    if(not missingTiles())
      notifications()->remove(NotificationModel::MissingTiles);
    else
      notifications()->add(NotificationModel::MissingTiles, true);
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

    if(not intersecting())
      notifications()->remove(NotificationModel::Intersecting);
    else
      notifications()->add(NotificationModel::Intersecting, true);
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

    if(matchingMetrics())
      notifications()->remove(NotificationModel::MetricsMismatch);
    else
      notifications()->add(NotificationModel::MetricsMismatch, false);
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
   * \property ElevationChartItem::notifications
   * \brief Модель оповещений для интерфейса.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>notifications</i>       <td><i>--</i>           <td><i>--</i>
   * </table>
   * \see ElevationChart::NotificationModel
   */
  NotificationModel* ElevationChartItem::notifications() const { return m_notifications; }

  /**
   * \property ElevationChartItem::uavPosition
   * \brief Позиция БПЛА в данный момент.
   * \details
   * Используется для вычисления относительной высоты и отображении позиции БПЛА на графике.
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

  Envelope ElevationChartItem::envelope() const { return m_envelope; }
  void ElevationChartItem::setEnvelope(const Envelope& x) {
    m_envelope = x;
    emit envelopeChanged();
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

  /**
   * \property ElevationChartItem::providerType
   * \brief Тип провайдера высотных данных.
   * \details
   * <i>По умолчанию равняется ProviderType::DEMProvider</i>.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>shrinkMode</i>  <td><i>--</i><td><i>--</i>
   * </table>
   * \see ProviderType
   */
  int ElevationChartItem::providerType() const { return static_cast<int>(m_provider_type); }

  map<ElevationChartItem::NodeTypes, QSGGeometryNode*>& ElevationChartItem::tree() { return m_tree; }
  IElevationDataProvider* ElevationChartItem::provider() { return m_provider.get(); }

  /**
   * \brief Возвращает указатель на дочерний объект Researcher.
   * \return Указатель на Researcher.
   */
  Researcher* ElevationChartItem::researcher() const { return m_researcher; }

  bool ElevationChartItem::allowEnvelopeCorrection() const { return m_envelope_route.valid(); }
} // ElevationChart