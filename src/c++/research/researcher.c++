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

class Point
{
  public:
    enum Orientation
    {
      Air,
      Ground,
      ToGround,
      FromGround
    };

    Point()
      : m_altitude(0)
      , m_distance(0)
      , m_base(false)
      , m_latitude(0)
      , m_longitude(0)
      , m_orientation(Air)
    {}

    Point(const Point& other)
      : m_altitude(other.altitude())
      , m_distance(other.distance())
      , m_base(other.base())
      , m_latitude(other.latitude())
      , m_longitude(other.longitude())
      , m_orientation(other.orientation())
    {}

    [[nodiscard]] float altitude() const { return m_altitude; }
    void setAltitude(float x) { m_altitude = x; }

    [[nodiscard]] float distance() const { return m_distance; }
    void setDistance(float x) { m_distance = x; }

    [[nodiscard]] bool base() const { return m_base; }
    void setBase(bool x) { m_base = x; }

    [[nodiscard]] double latitude() const { return m_latitude; }
    void setLatitude(double x) { m_latitude = x; }

    [[nodiscard]] double longitude() const { return m_longitude; }
    void setLongitude(double x) { m_longitude = x; }

    [[nodiscard]] Orientation orientation() const { return m_orientation; }
    void setOrientation(Orientation x) { m_orientation = x; }

  private:
    float m_altitude;
    float m_distance;
    bool m_base;
    double m_latitude;
    double m_longitude;
    Orientation m_orientation;
};

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
        QVector<Point> path_profile;
        QList<QGeoCoordinate> point_list = path.path();
        float distance = 0;
        for(int i = 0; i < point_list.size(); i++)
        {
          if(i)
            distance += static_cast<float>(path.length(i - 1, i));
          QGeoCoordinate point = point_list[i];

          Point new_point;
          new_point.setAltitude(point.altitude());
          new_point.setBase(true);
          new_point.setDistance(distance);
          new_point.setLatitude(point.latitude());
          new_point.setLongitude(point.longitude());
          path_profile.append(new_point);
        }

        QVector<Point> ground_profile;
        point_list = profile.path();
        distance = 0;
        for(int i = 0; i < point_list.size(); i++)
        {
          if(i)
            distance += static_cast<float>(profile.length(i - 1, i));
          QGeoCoordinate point = point_list[i];

          Point new_point;
          new_point.setAltitude(point.altitude());
          new_point.setBase(true);
          new_point.setDistance(distance);
          new_point.setLatitude(point.latitude());
          new_point.setLongitude(point.longitude());
          ground_profile.append(new_point);
        }

        QVector<Point> result_path;
        for(auto point : path_profile)
        {
          if(result_path.size())
          {
            Point prev_point = result_path.last();

            if(ground_profile.size() > 1)
            {
              for(int i = 1; i < ground_profile.size(); i++)
              {
                QPointF intersection_point;
                Point prev_ground_point = ground_profile[i - 1];
                Point ground_point = ground_profile[i];
                if(QLineF(prev_point.distance(), prev_point.altitude(), point.distance(), point.altitude()).intersects(
                    QLineF(prev_ground_point.distance(), prev_ground_point.altitude(), ground_point.distance(), ground_point.altitude()), &intersection_point)
                    == QLineF::BoundedIntersection)
                {
                  QGeoCoordinate path_geo_point = QGeoCoordinate(prev_point.latitude(), prev_point.longitude());
                  double azimuth = path_geo_point.azimuthTo(QGeoCoordinate(prev_point.latitude(), prev_point.longitude()));
                  QGeoCoordinate intersection_geo_point = path_geo_point.atDistanceAndAzimuth(intersection_point.x() - prev_point.distance(), azimuth);

                  Point prev_result_point = result_path.last();

                  Point new_point;
                  new_point.setAltitude(intersection_point.y());
                  new_point.setBase(false);
                  new_point.setDistance(intersection_point.x());
                  new_point.setLatitude(intersection_geo_point.latitude());
                  new_point.setLongitude(intersection_geo_point.longitude());
                  if(prev_result_point.orientation() == Point::Ground or prev_result_point.orientation() == Point::ToGround)
                    new_point.setOrientation(Point::FromGround);
                  else
                    new_point.setOrientation(Point::ToGround);
                  result_path.append(new_point);
                }
              }
            }
          }

          if(point.altitude() > DEM::elevation(point.latitude(), point.longitude()))
            point.setOrientation(Point::Air);
          else
            point.setOrientation(Point::Ground);
          result_path.append(point);
        }

        for(const auto& point : result_path)
          result.emplace_back(point.distance(), point.altitude());

        qDebug() << result.size();

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
