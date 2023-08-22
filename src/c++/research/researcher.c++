/**
  *  @file FILE
  *  @author Dmitry Ryazancev
  *  @date 16.08.23
  *  @copyright Radar-MMS 2023
  */

#include "researcher.h"
#include <QtConcurrent/QtConcurrent>
#include <DEM/Algorithms>
#include "types/intersectionpoint.h"

constexpr static float SCAN_STEP = 0.5;             // Шаг сканирования земной поверхности.

namespace ElevationChart
{
  Researcher::Researcher(QObject* parent)
    : QObject(parent)
  {
    qRegisterMetaType<vector<IntersectionPoint>>("vector<IntersectionPoint>");
  }

  void Researcher::researchIntersections(const QGeoPath& path)
  {
    QFuture<void> outer = QtConcurrent::run([this, path](){
      QFuture<void> inner = QtConcurrent::run([this, path](){
        vector<ElevationPoint> result;
        DEM::loadTiles(path);
        QGeoPath profile = Researcher::plotGeopathProfile(path);
        QVector<IntersectionPoint> path_profile;
        QList<QGeoCoordinate> point_list = path.path();
        float distance = 0;
        for(int i = 0; i < point_list.size(); i++)
        {
          if(i)
            distance += static_cast<float>(path.length(i - 1, i));
          QGeoCoordinate point = point_list[i];

          IntersectionPoint new_point(distance, static_cast<float>(point.altitude()), true, true, IntersectionPoint::NonIntersecting);
          new_point.setCoordinate({point.latitude(), point.longitude()});
          path_profile.append(new_point);
        }

        QVector<IntersectionPoint> ground_profile;
        point_list = profile.path();
        distance = 0;
        for(int i = 0; i < point_list.size(); i++)
        {
          if(i)
            distance += static_cast<float>(profile.length(i - 1, i));
          QGeoCoordinate point = point_list[i];

          IntersectionPoint new_point(distance, static_cast<float>(point.altitude()), true, true, IntersectionPoint::NonIntersecting);
          new_point.setCoordinate({point.latitude(), point.longitude()});
          ground_profile.append(new_point);
        }

        QVector<IntersectionPoint> result_path;
        for(auto point : path_profile)
        {
          if(not result_path.empty())
          {
            IntersectionPoint prev_point = result_path.last();

            if(ground_profile.size() > 1)
            {
              for(int i = 1; i < ground_profile.size(); i++)
              {
                QPointF intersection_point;
                IntersectionPoint prev_ground_point = ground_profile[i - 1];
                IntersectionPoint ground_point = ground_profile[i];
                if(QLineF(prev_point.distance(), prev_point.elevation(), point.distance(), point.elevation()).intersects(
                    QLineF(prev_ground_point.distance(), prev_ground_point.elevation(), ground_point.distance(), ground_point.elevation()), &intersection_point)
                    == QLineF::BoundedIntersection)
                {
                  QGeoCoordinate path_geo_point = prev_point.coordinate();
                  double azimuth = path_geo_point.azimuthTo(prev_point.coordinate());
                  QGeoCoordinate intersection_geo_point = path_geo_point.atDistanceAndAzimuth(intersection_point.x() - prev_point.distance(), azimuth);

                  IntersectionPoint prev_result_point = result_path.last();

                  IntersectionPoint new_point(intersection_point.x(), intersection_point.y(), true, false, IntersectionPoint::NonIntersecting);
                  new_point.setCoordinate(intersection_geo_point);
                  if(prev_result_point.state() == IntersectionPoint::InsideGround or prev_result_point.state() == IntersectionPoint::IntersectingIn)
                    new_point.setState(IntersectionPoint::IntersectingOut);
                  else
                    new_point.setState(IntersectionPoint::IntersectingIn);
                  result_path.append(new_point);
                }
              }
            }
          }

          if(point.elevation() > static_cast<float>(DEM::elevation(point.coordinate().latitude(), point.coordinate().longitude())))
            point.setState(IntersectionPoint::NonIntersecting);
          else
            point.setState(IntersectionPoint::InsideGround);
          result_path.append(point);
        }

        for(const auto& point : result_path)
        {
          if(not point.base())
            result.emplace_back(point.distance(), point.elevation());
          if(point.base() and point.state() == IntersectionPoint::InsideGround)
          {
            result.emplace_back(point.distance(), point.elevation());
            result.emplace_back(point.distance(), point.elevation());
          }
        }

        emit researchIntersectionsFinished(std::move(result));
      });
    });
  }

  QGeoPath Researcher::plotGeopathProfile(const QGeoPath& path)
  {
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

  float Researcher::altitudeAtDistance(const QGeoCoordinate& a, const QGeoCoordinate& b, int distance)
  {
    auto delta = a.distanceTo(b);
    auto altitudeDelta = std::abs(a.altitude() - b.altitude());
    if (a.altitude() > b.altitude())
    {
      auto tg = (a.altitude() - altitudeDelta) / delta;
      return static_cast<float>(b.altitude() + tg * (delta - distance));
    }

    else if (a.altitude() < b.altitude())
    {
      auto tg = (b.altitude() - altitudeDelta)/ delta;
      return static_cast<float>(a.altitude() + tg * distance);
    }
    return static_cast<float>(a.altitude());
  }
} // ElevationChart
