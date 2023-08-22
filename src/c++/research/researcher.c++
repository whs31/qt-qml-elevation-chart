/**
  *  @file FILE
  *  @author Dmitry Ryazancev
  *  @date 16.08.23
  *  @copyright Radar-MMS 2023
  */

#include "researcher.h"
#include <QtConcurrent/QtConcurrent>
#include <DEM/Algorithms>

#define in :
#define CONCURRENT_RUN_START_2(arg1, arg2) QFuture<void> future = QtConcurrent::run([arg1, arg2](){ \
                                    QFuture<void> future_priv = QtConcurrent::run([arg1, arg2](){
#define CONCURRENT_RUN_START_3(arg1, arg2, arg3) QFuture<void> future = QtConcurrent::run([arg1, arg2, arg3](){ \
                                    QFuture<void> future_priv = QtConcurrent::run([arg1, arg2, arg3](){
#define CONCURRENT_RUN_END });});
#define CONCURRENT_RUN_END_WATCHER(watcher) CONCURRENT_RUN_END \
                                            watcher.setFuture(future);

constexpr static float SCAN_STEP = 0.5; // Шаг сканирования земной поверхности.
constexpr static float BOUND_RATE_ANGLE = 5;

namespace ElevationChart
{
  Researcher::Researcher(QObject* parent)
    : QObject(parent)
    , m_busy(false)
    , m_busy2(false)
  {
    qRegisterMetaType<vector<IntersectionPoint>>("vector<IntersectionPoint>");
    qRegisterMetaType<EnvelopeResult>("EnvelopeResult");

    connect(&m_watcher, &QFutureWatcher<void>::finished, this, [this](){ m_busy = false; emit busyChanged(); });
    connect(&m_watcher, &QFutureWatcher<void>::started, this, [this](){ m_busy = true; emit busyChanged(); });
    connect(&m_watcher2, &QFutureWatcher<void>::finished, this, [this](){ m_busy2 = false; emit busyChanged(); });
    connect(&m_watcher2, &QFutureWatcher<void>::started, this, [this](){ m_busy2 = true; emit busyChanged(); });
  }

  void Researcher::researchIntersections(const QGeoPath& path)
  {
    CONCURRENT_RUN_START_2(this, path)
      QGeoPath profile = Researcher::plotGeopathProfile(path);
      vector<IntersectionPoint> path_profile = fillProfile(path.path(), path);
      vector<IntersectionPoint> ground_profile = fillProfile(profile.path(), profile);
      vector<IntersectionPoint> result_path;
      for(auto point : path_profile)
      {
        if(not result_path.empty())
        {
          if(ground_profile.size() > 1)
          {
            for(int i = 1; i < ground_profile.size(); i++)
            {
              QPointF f;
              QLineF a(result_path.back().distance(), result_path.back().elevation(), point.distance(), point.elevation());
              QLineF b(ground_profile[i - 1].distance(), ground_profile[i - 1].elevation(), ground_profile[i].distance(), ground_profile[i].elevation());
              if(a.intersects(b, &f) == QLineF::BoundedIntersection)
                result_path.emplace_back(static_cast<float>(f.x()), static_cast<float>(f.y()), true, false,
                                         (result_path.back().state() == IntersectionPoint::InsideGround
                                          or result_path.back().state() == IntersectionPoint::IntersectingIn)
                                         ? IntersectionPoint::IntersectingIn : IntersectionPoint::IntersectingOut,
                                         result_path.back().coordinate().atDistanceAndAzimuth(f.x() - result_path.back().distance(),
                                         result_path.back().coordinate().azimuthTo(result_path.back().coordinate()))
                );
            }
          }
        }

        point.setState((point.elevation() > static_cast<float>(DEM::elevation(point.coordinate().latitude(), point.coordinate().longitude()))) ?
                       IntersectionPoint::NonIntersecting : IntersectionPoint::InsideGround);
        result_path.push_back(point);
      }

      // removing useless points from result and doubling some points for GL_QUADS mode
      vector<ElevationPoint> result;
      result.reserve(result_path.size());
      for(const auto& point : result_path)
      {
        if(not point.base())
          result.emplace_back(point.distance(), point.elevation());
        else if(point.state() == IntersectionPoint::InsideGround)
        {
          result.emplace_back(point.distance(), point.elevation());
          result.emplace_back(point.distance(), point.elevation());
        }
      }

      emit researchIntersectionsFinished(std::move(result));
    CONCURRENT_RUN_END_WATCHER(m_watcher)
  }

  void Researcher::researchEnvelope(const QGeoPath& path, const Metrics& metrics, const Envelope& envelope)
  {
    CONCURRENT_RUN_START_3(path, metrics, envelope)
    
    CONCURRENT_RUN_END_WATCHER(m_watcher2)
  }

  QGeoPath Researcher::plotGeopathProfile(const QGeoPath& path)
  {
    DEM::loadTiles(path);
    QGeoPath ret;
    for(QGeoCoordinate point : path.path())
    {
      if(ret.size())
      {
        QGeoCoordinate previous = ret.coordinateAt(ret.size() - 1);
        auto azimuth = static_cast<float>(previous.azimuthTo(point));
        auto distance_from_previous = static_cast<float>(previous.distanceTo(point));
        float distance = SCAN_STEP;
        QGeoCoordinate previous_delta = previous;

        while(distance < distance_from_previous)
        {
          QGeoCoordinate delta_point = previous.atDistanceAndAzimuth(distance, azimuth);
          delta_point.setAltitude(DEM::elevation(delta_point.latitude(), delta_point.longitude()));

          if(previous_delta.altitude() != delta_point.altitude())
          {
            if(previous_delta.altitude() > delta_point.altitude())
              ret.addCoordinate(previous_delta);
            else
              ret.addCoordinate(delta_point);
          }

          previous_delta = delta_point;
          distance += SCAN_STEP;
        }
      }

      point.setAltitude(DEM::elevation(point.latitude(), point.longitude()));
      ret.addCoordinate(point);
    }

    return ret;
  }

  vector<ElevationChart::IntersectionPoint> Researcher::fillProfile(const QList<QGeoCoordinate>& list, const QGeoPath& path)
  {
    vector<ElevationChart::IntersectionPoint> ret;

    float distance = 0;
    for(int i = 0; i < list.size(); i++)
    {
      if(i)
        distance += static_cast<float>(path.length(i - 1, i));
      const QGeoCoordinate& point = list[i];
      ret.emplace_back(distance,
                       static_cast<float>(point.altitude()),
                       true, true,
                       ElevationChart::IntersectionPoint::NonIntersecting,
                       QGeoCoordinate(point.latitude(), point.longitude())
      );
    }

    return ret;
  }

  vector<ElevationChart::IntersectionPoint> Researcher::createRawGroundPath(const QGeoPath& path)
  {
    QGeoPath raw_ground_geopath;
    vector<ElevationChart::IntersectionPoint> ret;

    float distance_from_start = 0;
    for(auto point in path.path())
    {
      if(not raw_ground_geopath.isEmpty())
      {
        QGeoCoordinate prev_base_geopoint = raw_ground_geopath.coordinateAt(raw_ground_geopath.size() - 1);
        float distance = SCAN_STEP;
        QGeoCoordinate prev_delta_geopoint = prev_base_geopoint;
        while(distance < static_cast<float>(prev_base_geopoint.distanceTo(point)))
        {
          QGeoCoordinate delta_geopoint = prev_base_geopoint.atDistanceAndAzimuth(distance, static_cast<float>(prev_base_geopoint.azimuthTo(point)));
          delta_geopoint.setAltitude(DEM::elevation(delta_geopoint.latitude(), delta_geopoint.longitude()));

          if(prev_delta_geopoint.altitude() == delta_geopoint.altitude())
          {
            prev_delta_geopoint = delta_geopoint;
            distance += SCAN_STEP;

            continue;
          }

          if(prev_delta_geopoint.altitude() > delta_geopoint.altitude())
          {
            raw_ground_geopath.addCoordinate(prev_delta_geopoint);
            if(raw_ground_geopath.size() > 1)
              distance_from_start += static_cast<float>(raw_ground_geopath.length(raw_ground_geopath.size() - 2, raw_ground_geopath.size() - 1));
            ret.emplace_back(distance_from_start, prev_delta_geopoint.altitude(), true, false, ElevationChart::IntersectionPoint::NonIntersecting, prev_delta_geopoint);
          }
          else
          {
            raw_ground_geopath.addCoordinate(delta_geopoint);
            if(raw_ground_geopath.size() > 1)
              distance_from_start += static_cast<float>(raw_ground_geopath.length(raw_ground_geopath.size() - 2, raw_ground_geopath.size() - 1));
            ret.emplace_back(distance_from_start, delta_geopoint.altitude(), true, false, ElevationChart::IntersectionPoint::NonIntersecting, delta_geopoint);
          }
        }
      }

      point.setAltitude(DEM::elevation(point.latitude(), point.longitude()));
      raw_ground_geopath.addCoordinate(point);
      if(raw_ground_geopath.size() > 1)
        distance_from_start += static_cast<float>(raw_ground_geopath.length(raw_ground_geopath.size() - 2, raw_ground_geopath.size() - 1));

      ret.emplace_back(distance_from_start, point.altitude(), true, true, ElevationChart::IntersectionPoint::NonIntersecting, point);
    }

    return ret;
  }

  bool Researcher::busy() const { return m_busy or m_busy2; }
} // ElevationChart
