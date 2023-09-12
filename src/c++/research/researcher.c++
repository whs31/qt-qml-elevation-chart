/**
  *  @file FILE
  *  @author Dmitry Ryazancev
  *  @date 16.08.23
  *  @copyright Radar-MMS 2023
  */

#include "researcher.h"
#include <deque>
#include <iterator>
#include <QtConcurrent/QtConcurrent>
#include <DEM/Algorithms>
#include "internal/definitions.h"

using std::deque;

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
    qRegisterMetaType<EnvelopeResult>("Researcher::EnvelopeResult");

    connect(&m_watcher, &QFutureWatcher<void>::finished, this, [this](){ m_busy = false; emit busyChanged(); });
    connect(&m_watcher, &QFutureWatcher<void>::started, this, [this](){ m_busy = true; emit busyChanged(); });
    connect(&m_watcher2, &QFutureWatcher<void>::finished, this, [this](){ m_busy2 = false; emit busyChanged(); });
    connect(&m_watcher2, &QFutureWatcher<void>::started, this, [this](){ m_busy2 = true; emit busyChanged(); });
  }

  void Researcher::researchIntersections(const QGeoPath& path)
  {
    CONCURRENT_RUN [this, path] CONCURRENT_ARGS [this, path] CONCURRENT_RUN_START
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

  void pushLH(const IntersectionPoint& l, const IntersectionPoint& h, vector<IntersectionPoint>& inter_vec, vector<ElevationPoint>& ele_vec)
  {
    inter_vec.push_back(l);
    ele_vec.push_back(l);
    ele_vec.push_back(h);
  }

  void Researcher::researchEnvelope(const QGeoPath& path, const Metrics& metrics, const Envelope& envelope)
  {
    CONCURRENT_RUN [this, path, metrics, envelope] CONCURRENT_ARGS [this, path, metrics, envelope] CONCURRENT_RUN_START
      EnvelopeResult res;

      vector<IntersectionPoint> ground_path = createRawGroundPathLegacy(path);
      vector<IntersectionPoint> low_bound_path;
      if(not ground_path.empty())
      {
        const float ENVELOPE_LBP = envelope.altitude() - envelope.width() / 2.0f;
        const float ENVELOPE_HBP = envelope.altitude() + envelope.width() / 2.0f;

        IntersectionPoint lbp(ground_path.front().distance(), ground_path.front().elevation() + ENVELOPE_LBP, true, ground_path.front().base(),
                              IntersectionPoint::NonIntersecting, ground_path.front().coordinate());
        IntersectionPoint hbp(ground_path.front().distance(), ground_path.front().elevation() + ENVELOPE_HBP, true, ground_path.front().base(),
                              IntersectionPoint::NonIntersecting, ground_path.front().coordinate());
        pushLH(lbp, hbp, low_bound_path, res.boundPolygon);

        if(ground_path.size() > 1)
        {
          for(size_t i = 1; i < ground_path.size() - 1; i++)
          {
            lbp = ground_path[i];
            lbp.setElevation(lbp.elevation() + ENVELOPE_LBP);
            hbp = ground_path[i];
            hbp.setElevation(hbp.elevation() + ENVELOPE_HBP);

            if(lbp.base())
              pushLH(lbp, hbp, low_bound_path, res.boundPolygon);

            else
            {
              IntersectionPoint lbp_prev = low_bound_path[low_bound_path.size() - 1];
              IntersectionPoint lbp_next = ground_path[i + 1];
              lbp_next.setElevation(lbp_next.elevation() + ENVELOPE_LBP);

              float angle = Researcher::angle3point({lbp_prev.distance(), lbp_prev.elevation()},
                                                    {lbp.distance(), lbp.elevation()},
                                                    {lbp_next.distance(), lbp_next.elevation()});
              if(angle > BOUND_RATE_ANGLE)
                pushLH(lbp, hbp, low_bound_path, res.boundPolygon);
            }
          }

          lbp = ground_path[ground_path.size() - 1];
          lbp.setElevation(lbp.elevation() + ENVELOPE_LBP);
          hbp = ground_path[ground_path.size() - 1];
          hbp.setElevation(hbp.elevation() + ENVELOPE_HBP);
          pushLH(lbp, hbp, low_bound_path, res.boundPolygon);
        }
      }

      deque<IntersectionPoint> route_path;
      deque<IntersectionPoint> delta_bound;
      deque<IntersectionPoint> delta_route;

      if(low_bound_path.empty() or low_bound_path.size() == 1)
      {
        emit researchEnvelopeFinished(std::move(res));
        return;
      }

      IntersectionPoint route_point = low_bound_path.front();
      route_point.setElevation(route_point.elevation() + envelope.width() / 2.0f);
      res.route.add(RoutePoint(route_point.toQGeoCoordinate()));
      route_path.push_back(route_point);

      delta_bound.push_back(low_bound_path.front());
      delta_route.push_back(route_point);

      for(size_t i = 1; i < low_bound_path.size(); i++)
      {
        IntersectionPoint prev_route_point = route_path[route_path.size() - 1];
        route_point = low_bound_path[i];
        route_point.setElevation(route_point.elevation() + envelope.width() / 2.0f);

        const float dt = (route_point.distance() - prev_route_point.distance()) / metrics.fallbackVelocity();
        if(prev_route_point.elevation() < route_point.elevation())
        {
          if((route_point.elevation() - prev_route_point.elevation()) / dt > metrics.rateOfClimb())
            route_point.setElevation(prev_route_point.elevation() + dt * metrics.rateOfClimb());
        }
        else if(prev_route_point.elevation() > route_point.elevation())
        {
          if((prev_route_point.elevation() - route_point.elevation()) / dt > metrics.rateOfDescend())
            route_point.setElevation(prev_route_point.elevation() - dt * metrics.rateOfDescend());
        }

        delta_bound.push_back(low_bound_path[i]);
        delta_route.push_back(route_point);

        bool intersects = false;
        IntersectionPoint start_drp = delta_route.front();
        QLineF delta_route_line(start_drp.distance(), start_drp.elevation(), route_point.distance(), route_point.elevation());

        for(size_t j = 1; j < delta_bound.size(); j++)
        {
          IntersectionPoint start_delta_lbp = delta_bound[j - 1];
          IntersectionPoint end_delta_lbp = delta_bound[j];
          intersects = delta_route_line.intersects({start_delta_lbp.distance(), start_delta_lbp.elevation(),
                                                          end_delta_lbp.distance(), end_delta_lbp.elevation()},
                                                          nullptr) == QLineF::BoundedIntersection or
                       delta_route_line.intersects({start_delta_lbp.distance(), start_delta_lbp.elevation() + envelope.width(),
                                                          end_delta_lbp.distance(), end_delta_lbp.elevation() + envelope.width()},
                                                          nullptr) == QLineF::BoundedIntersection;
        }

        if(intersects)
        {
          if(delta_route.size() < 3)
          {
            res.route.add(RoutePoint(route_point.toQGeoCoordinate()));
            route_path.push_back(route_point);
            delta_bound.pop_front();
            delta_route.pop_front();
          }
          else
          {
            prev_route_point = delta_route[delta_route.size() - 2];
            res.route.add(RoutePoint(prev_route_point.toQGeoCoordinate()));
            route_path.push_back(prev_route_point);

            auto it1 = delta_bound.begin();
            auto it2 = delta_route.begin();
            std::advance(it1, delta_bound.size() - 2);
            std::advance(it2, delta_route.size() - 2);
            delta_bound.erase(delta_bound.begin(), it1);
            delta_route.erase(delta_route.begin(), it2);

            delta_bound.pop_back();
            delta_route.pop_back();
            i--;
          }
        }

        if(route_point.base() and delta_route.size() > 1)
        {
          res.route.add(RoutePoint(route_point.toQGeoCoordinate()));
          route_path.push_back(route_point);

          auto it1 = delta_bound.begin();
          auto it2 = delta_route.begin();
          std::advance(it1, delta_bound.size() - 1);
          std::advance(it2, delta_route.size() - 1);
          delta_bound.erase(delta_bound.begin(), it1);
          delta_route.erase(delta_route.begin(), it2);
        }
      }

      emit researchEnvelopeFinished(std::move(res));
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

  auto Researcher::createRawGroundPathLegacy(const QGeoPath& path) -> vector<IntersectionPoint>
  {
    vector<IntersectionPoint> ret;
    QGeoPath geo;
    float dfs = 0;
    for(auto point in path.path())
    {
      if(not geo.isEmpty())
      {
        QGeoCoordinate previous = geo.coordinateAt(geo.size() - 1);
        auto azimuth = static_cast<float>(previous.azimuthTo(point));
        auto distance_to_previous = static_cast<float>(previous.distanceTo(point));
        float distance = SCAN_STEP;
        QGeoCoordinate previous_delta = previous;
        while(distance < distance_to_previous)
        {
          QGeoCoordinate dpg = previous.atDistanceAndAzimuth(distance, azimuth);
          dpg.setAltitude(DEM::elevation(dpg.latitude(), dpg.longitude()));

          if(previous_delta.altitude() != dpg.altitude())
          {
            if(previous_delta.altitude() > dpg.altitude())
            {
              geo.addCoordinate(previous_delta);
              if(geo.size() > 1)
                dfs += static_cast<float>(geo.length(geo.size() - 2, geo.size() - 1));
              ret.emplace_back(dfs, static_cast<float>(previous_delta.altitude()), true, false, IntersectionPoint::NonIntersecting, previous_delta);
            }
            else
            {
              geo.addCoordinate(dpg);
              if(geo.size() > 1)
                dfs += static_cast<float>(geo.length(geo.size() - 2, geo.size() - 1));
              ret.emplace_back(dfs, static_cast<float>(dpg.altitude()), true, false, IntersectionPoint::NonIntersecting, dpg);
            }
          }
          previous_delta = dpg;
          distance += SCAN_STEP;
        }
      }
      point.setAltitude(DEM::elevation(point.latitude(), point.longitude()));
      geo.addCoordinate(point);
      int pointCount = geo.size();
      if (pointCount > 1)
        dfs += static_cast<float>(geo.length(pointCount - 2, pointCount - 1));
      ret.emplace_back(dfs, static_cast<float>(point.altitude()), true, true, IntersectionPoint::NonIntersecting, point);
    }
    return ret;
  }

  bool Researcher::busy() const { return m_busy or m_busy2; }

  float Researcher::angle3point(QPointF a, QPointF b, QPointF c)
  {
    QPointF ab = { b.x() - a.x(), b.y() - a.y() };
    QPointF cb = { b.x() - c.x(), b.y() - c.y() };
    return static_cast<float>(180 - std::abs(qRadiansToDegrees(std::atan2((ab.x() * cb.y() - ab.y() * cb.x()),
                                                                          (ab.x() * cb.x() + ab.y() * cb.y())))));
  }
} // ElevationChart
