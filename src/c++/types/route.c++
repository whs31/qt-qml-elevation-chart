#include "route.h"

namespace ElevationChart
{
  Route::Route()
    : m_vec()
  {}

  Route::Route(const QGeoPath& path, float velocity)
    : m_vec()
  {
    this->set(path, velocity);
  }

  Route::Route(const vector<RoutePoint>& vec)
    : m_vec(vec)
  {}

  void Route::add(const RoutePoint& point) { m_vec.push_back(point); }
  void Route::add(const QGeoCoordinate& coord, float velocity) { m_vec.emplace_back(coord, velocity); }
  void Route::remove(int index) { m_vec.erase(m_vec.begin() + index); }
  void Route::remove(const ElevationChart::RoutePoint& point)
  {
    for(size_t i = 0; i < size(); ++i)
      if(point.coordinate() == m_vec[i].coordinate() and qFuzzyCompare(point.velocity(), m_vec[i].velocity()))
        m_vec.erase(m_vec.begin() + i);
  }
  void Route::replace(int index, const ElevationChart::RoutePoint& point) { m_vec.at(index) = point; }
  void Route::clear() { m_vec.clear(); }
  void Route::set(const QGeoPath& path, float velocity)
  {
    for(const auto& coordinate : path.path())
      m_vec.emplace_back(coordinate, velocity);
  }
  void Route::set(const vector<ElevationChart::RoutePoint>& vec) { m_vec = vec; }
  void Route::setVelocity(float velocity)
  {
    for(auto& point : m_vec)
      point.setVelocity(velocity);
  }
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

  int Route::size() const { return m_vec.size(); }
  QGeoPath Route::toGeoPath() const
  {
    QGeoPath ret;
    for(const auto& point : m_vec)
      ret.addCoordinate(point.coordinate());
    return ret;
  }
  QVariantList Route::toVariantList() const
  {
    QVariantList ret;
    for(const auto& point : m_vec)
      ret.push_back(QVariant::fromValue(point.coordinate()));
    return ret;
  }
  RoutePoint Route::at(int index) const
  {
    if(index < 0 or index >= size())
      return {};
    return m_vec.at(index);
  }
  bool Route::contains(const ElevationChart::RoutePoint& x)
  {
    return std::any_of(m_vec.cbegin(), m_vec.cend(), [x](const RoutePoint& point){
      return (point.coordinate() == x.coordinate() and qFuzzyCompare(point.velocity(), x.velocity()));
    });
  }
  bool Route::valid() const { return not m_vec.empty(); }
  Route Route::fromGeoPath(const QGeoPath& path, float velocity) { return Route(path, velocity); }
  Route Route::fromPointsVector(const vector<ElevationChart::RoutePoint>& vec) { return Route(vec); }
} // ElevationChart