/**
  *  @file FILE
  *  @author Dmitry Ryazancev
  *  @date 16.08.23
  *  @copyright Radar-MMS 2023
  */

#include "researcher.h"
#include <QtConcurrent/QtConcurrent>
#include <DEM/Algorithms>

constexpr static float SCAN_STEP = 1;             // Шаг сканирования земной поверхности.

namespace ElevationChart
{
  Researcher::Researcher(QObject* parent)
    : QObject(parent)
  {}

  void Researcher::researchIntersections(const QGeoPath& path)
  {
    QFuture<void> outer = QtConcurrent::run([this, path](){
      QFuture<void> inner = QtConcurrent::run([this, path](){
        QGeoPath profile = Researcher::plotGeopathProfile(path);

        vector<IntersectionPoint> profile_path;
        QList<QGeoCoordinate> points = path.path();
        float distance = 0;

        for(int i = 0; i < points.size(); i++)
        {
          if(i)
            distance += static_cast<float>(path.length(i - 1, i));
          QGeoCoordinate point = points[i];

          IntersectionPoint intersection_point(distance, static_cast<float>(point.altitude()), true,
                                               true, IntersectionPoint::NonIntersecting);
          intersection_point.setCoordinate({point.latitude(), point.longitude()});
          profile_path.push_back(intersection_point);
        }

        vector<IntersectionPoint> groundProfile;
        points = profile.path();
        distance = 0;
        for(int i = 0; i < points.size(); i++)
        {
          if(i)
            distance += static_cast<float>(path.length(i - 1, i));
          QGeoCoordinate point = points[i];

          IntersectionPoint intersection_point(distance, static_cast<float>(point.altitude()),
                                                 true, true, IntersectionPoint::NonIntersecting);
          intersection_point.setCoordinate({point.latitude(), point.longitude()});
          groundProfile.push_back(intersection_point);
        }

        vector<IntersectionPoint> result;
        for(auto& point : profile_path)
        {
          if(not result.empty())
          {
            IntersectionPoint prevPoint = result.back();

            if(groundProfile.size() > 1)
            {
              for(int i = 1; i < groundProfile.size(); i++)
              {
                QPointF intersectsPoint;
                IntersectionPoint prevGroundPoint = groundProfile[i - 1];
                IntersectionPoint groundPoint = groundProfile[i];
                QLineF line(prevPoint.distance(), prevPoint.elevation(), point.distance(), point.elevation());

                if(line.intersects(QLineF(prevGroundPoint.distance(), prevGroundPoint.elevation(), groundPoint.distance(), groundPoint.elevation()),
                                   &intersectsPoint) == QLineF::BoundedIntersection)
                {
                  QGeoCoordinate pathGeoPoint = QGeoCoordinate(prevPoint.coordinate().latitude(), prevPoint.coordinate().longitude());
                  float az = static_cast<float>(pathGeoPoint.azimuthTo(QGeoCoordinate(point.coordinate().latitude(), point.coordinate().longitude())));
                  QGeoCoordinate intersectGeoPoint = pathGeoPoint.atDistanceAndAzimuth(intersectsPoint.x() - prevPoint.distance(), az);

                  IntersectionPoint prevResultPoint = result.back();

                  IntersectionPoint intersection_point(static_cast<float>(intersectsPoint.x()), static_cast<float>(intersectsPoint.y()),
                                                       true, false, IntersectionPoint::NonIntersecting);
                  intersection_point.setCoordinate({intersectGeoPoint.latitude(), intersectGeoPoint.longitude()});
                  if(prevResultPoint.state() == IntersectionPoint::InsideGround)
                    intersection_point.setState(IntersectionPoint::IntersectingOut);
                  else
                    intersection_point.setState(IntersectionPoint::IntersectingIn);

                  result.push_back(intersection_point);
                }
              }
            }
          }

          if(point.elevation() > static_cast<float>(DEM::elevation(point.coordinate().latitude(), point.coordinate().longitude())))
            point.setState(IntersectionPoint::NonIntersecting);
          else
            point.setState(IntersectionPoint::InsideGround);

          result.push_back(point);
        }

        emit researchIntersectionsFinished(result);
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

          if (previous_delta.altitude() != delta_point.altitude())
          {
            if (previous_delta.altitude() > delta_point.altitude())
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
