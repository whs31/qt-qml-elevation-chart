#include "route.h"

namespace ElevationChart
{
  /**
   * \class Route
   * \brief Набор последовательных точек на карте.
   * \details Класс может быть создан как базовый тип в QML.
   * \see TypeFactory::route
   */

  /**
   * \brief Создает пустой путь.
   */
  Route::Route()
    : m_vec()
  {}

  /**
   * \brief Создает путь из данного QGeoPath с константной скоростью точек.
   * \param path - набор координат с высотой.
   * \param velocity - константная скорость для точек в метрах в секунду.
   * <i>По умолчанию равно нулю</i>.
   */
  Route::Route(const QGeoPath& path, float velocity)
    : m_vec()
  {
    this->set(path, velocity);
  }

  /**
   * \brief Создает путь из данного вектора точек.
   * \param vec - вектор точек RoutePoint.
   */
  Route::Route(const vector<RoutePoint>& vec)
    : m_vec(vec)
  {}

  /**
   * \brief Добавляет в конец пути точку.
   * \param point - точка для добавления.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::add(const RoutePoint& point) { m_vec.push_back(point); }

  /**
   * \brief Добавляет в конец пути точку, созданную из координаты и значения скорости.
   * \param coord - координата точки с высотой.
   * \param velocity - скорость точки в метрах в секунду.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::add(const QGeoCoordinate& coord, float velocity) { m_vec.emplace_back(coord, velocity); }

  /**
   * \brief Удаляет из пути точку по индексу.
   * \param index - индекс точки для удаления.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::remove(int index) { m_vec.erase(m_vec.begin() + index); }

  /**
   * \brief Удаляет из пути точку по значению
   * \param point - значение точки для удаления.
   * \warning Удалено будет только первое вхождение.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::remove(const ElevationChart::RoutePoint& point)
  {
    for(size_t i = 0; i < size(); ++i)
      if(point.coordinate() == m_vec[i].coordinate() and qFuzzyCompare(point.velocity(), m_vec[i].velocity()))
        m_vec.erase(m_vec.begin() + i);
  }

  /**
   * \brief Заменяет точку по индексу.
   * \param index - индекс точки для замены.
   * \param point - новое значение точки.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::replace(int index, const ElevationChart::RoutePoint& point) { m_vec.at(index) = point; }

  /**
   * \brief Очищает путь.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::clear() { m_vec.clear(); }

  /**
   * \brief Задает путь из QGeoPath с константной скоростью точек.
   * \param path - набор координат с высотой.
   * \param velocity - константная скорость для точек в метрах в секунду.
   * <i>По умолчанию равно нулю</i>.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::set(const QGeoPath& path, float velocity)
  {
    for(const auto& coordinate : path.path())
      m_vec.emplace_back(coordinate, velocity);
  }

  /**
   * \brief Задает путь из вектора точек.
   * \param vec - вектор точек RoutePoint.
   */
  void Route::set(const vector<ElevationChart::RoutePoint>& vec) { m_vec = vec; }

  /**
   * \brief Задает константную общую скорость каждой точке в пути.
   * \param velocity - скорость в метрах в секунду.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::setVelocity(float velocity)
  {
    for(auto& point : m_vec)
      point.setVelocity(velocity);
  }

  /**
   * \brief Задает индивидуальную скорость из массива для каждой точки.
   * \param velocities - массив скоростей точек в метрах в секунду.
   * \param fallback - значение, которое будет использоваться, если массив
   * скоростей меньше, чем количество точек в пути (м/с). <i>По умолчанию равно нулю</i>.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void Route::setVelocity(const vector<float>& velocities, float fallback)
  {
    for(size_t i = 0; i < size(); ++i)
    {
      if(i < velocities.size())
        m_vec.at(i).setVelocity(velocities.at(i));
      else
        m_vec.at(i).setVelocity(fallback);
    }
  }

  /**
   * \brief Возвращает количество точек в пути.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   * \return Количество точек.
   */
  int Route::size() const { return m_vec.size(); }

  /**
   * \brief Преобразует путь в QGeoPath и возвращает его.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   * \return QGeoPath
   */
  QGeoPath Route::toGeoPath() const
  {
    QGeoPath ret;
    for(const auto& point : m_vec)
      ret.addCoordinate(point.coordinate());
    return ret;
  }

  /**
   * \brief Преобразует путь в QVariantList и возвращает его.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   * \return QVariantList
   */
  QVariantList Route::toVariantList() const
  {
    QVariantList ret;
    for(const auto& point : m_vec)
      ret.push_back(QVariant::fromValue(point.coordinate()));
    return ret;
  }

  /**
   * \brief Возвращает точку, лежащую по индексу, или пустую точку, если индекс невалидный.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   * \return Точка пути RoutePoint
   */
  RoutePoint& Route::at(int index) { return m_vec.at(index); }

  /**
   * \brief Возвращает <tt>true</tt>, если путь содержит указанную точку.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  bool Route::contains(const ElevationChart::RoutePoint& x)
  {
    return std::any_of(m_vec.cbegin(), m_vec.cend(), [x](const RoutePoint& point){
      return (point.coordinate() == x.coordinate() and qFuzzyCompare(point.velocity(), x.velocity()));
    });
  }

  /**
   * \brief Возвращает <tt>true</tt>, если путь корректный.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  bool Route::valid() const { return not m_vec.empty(); }

  /**
   * \brief Создает путь из данного QGeoPath с константной скоростью точек.
   * \param path - набор координат с высотой.
   * \param velocity - константная скорость для точек в метрах в секунду.
   * <i>По умолчанию равно нулю</i>.
   */
  Route Route::fromGeoPath(const QGeoPath& path, float velocity) { return Route(path, velocity); }

  /**
   * \brief Создает путь из данного вектора точек.
   * \param vec - вектор точек RoutePoint.
   */
  Route Route::fromPointsVector(const vector<ElevationChart::RoutePoint>& vec) { return Route(vec); }

  /**
   * \brief Преобразует путь в вектор точек ElevationPoint.
   */
  auto Route::toElevationGraph() const -> vector<ElevationPoint>
  {
    vector<ElevationPoint> ret;
    auto prev_coord = m_vec.front().coordinate();
    float distance = 0;
    for(const auto& point : m_vec)
    {
      distance += static_cast<float>(point.coordinate().distanceTo(prev_coord));
      ret.emplace_back(distance, point.altitude());
      prev_coord = point.coordinate();
    }

    return ret;
  }

  /**
   * \brief Возвращает массив скоростей всех точек пути.
   * \return Массив индивидуальных скоростей точек.
   */
  auto Route::velocities() const -> vector<float>
  {
    vector<float> ret;
    for(const auto& point : m_vec)
      ret.push_back(point.velocity());
    return ret;
  }
} // ElevationChart