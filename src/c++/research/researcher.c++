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

vector<ElevationChart::IntersectionPoint> fillProfile(const QList<QGeoCoordinate>& list, const QGeoPath& path)
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
} // ElevationChart
