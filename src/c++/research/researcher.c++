/**
  *  @file FILE
  *  @author Dmitry Ryazancev
  *  @date 16.08.23
  *  @copyright Radar-MMS 2023
  */

#include "researcher.h"
#include <QtConcurrent/QtConcurrent>
#include <DEM/Algorithms>

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
        DEM::loadTiles(path);
        const int distanceDelta = 10;
        const qreal altitudeDelta = 0.0000001;
        vector<ElevationPoint> result{};
        qreal resultDistance = 0;

//      auto profilePath = DEM::buildProfile(path);
        for (int i = 0; i < path.size() - 1; ++i) {
          auto firstPoint = path.coordinateAt(i);
          auto secondPoint = path.coordinateAt(i + 1);
          auto distance = firstPoint.distanceTo(secondPoint);
          auto azimuth = firstPoint.azimuthTo(secondPoint);
          for (int calcDistance = 0; calcDistance < distance; calcDistance += distanceDelta) {
            if (calcDistance == 0) {

              if (firstPoint.altitude() + altitudeDelta <
                  DEM::elevation(firstPoint.latitude(), firstPoint.longitude())) {
                qDebug() << firstPoint.altitude() + altitudeDelta << DEM::elevation(firstPoint.latitude(), firstPoint.longitude());
                result.emplace_back(resultDistance + 0, firstPoint.altitude());
                continue;
              }


            }
            auto calcPoint = firstPoint.atDistanceAndAzimuth(calcDistance, azimuth);
            calcPoint.setAltitude(altitudeAtDistance(firstPoint, secondPoint, calcDistance));
            if (calcPoint.altitude() + altitudeDelta <
                DEM::elevation(calcPoint.latitude(), calcPoint.longitude())) {
              qDebug() << calcPoint.altitude() + altitudeDelta << DEM::elevation(calcPoint.latitude(), calcPoint.longitude());
              result.emplace_back(resultDistance + calcDistance, calcPoint.altitude());
            }

            if (calcDistance + distanceDelta >= distance) {
              if (secondPoint.altitude() + altitudeDelta <
                  DEM::elevation(secondPoint.latitude(), secondPoint.longitude())) {
                qDebug() << secondPoint.altitude() + altitudeDelta <<  DEM::elevation(calcPoint.latitude(), calcPoint.longitude());
                result.emplace_back(resultDistance + distance, secondPoint.altitude());
              }
              resultDistance += distance;
            }
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
