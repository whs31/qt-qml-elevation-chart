#pragma once

#include <QtPositioning/QGeoCoordinate>

namespace ElevationChart
{
  /**
   * \brief Точка пути Route на карте.
   * \details Представляет собой объект, хранящий координаты точки, ее высоту
   * и скорость в метрах в секунду.
   * \see Route
   */
  class RoutePoint
  {
    Q_GADGET
    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate FINAL)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude STORED false FINAL)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude STORED false FINAL)
    Q_PROPERTY(float altitude READ altitude WRITE setAltitude STORED false FINAL)
    Q_PROPERTY(float velocity READ velocity WRITE setVelocity FINAL)

    public:
      /// \brief Создает точку с невалидными координатами и нулевой скоростью.
      RoutePoint();

      /**
       * \brief Создает точку с указанными координатами, высотой и скоростью.
       * \param latitude - широта точки.
       * \param longitude - долгота точки.
       * \param elevation - высота точки в метрах. <i>По умолчанию равна нулю.</i>
       * \param velocity_ms - скорость точки в метрах в секунду. <i>По умолчанию равна нулю.</i>
       */
      RoutePoint(double latitude, double longitude, float elevation = 0, float velocity_ms = 0);

      /**
       * \brief Создает точку с заданной скоростью из объекта QGeoCoordinate.
       * \param coord - координаты, включая высоту в метрах.
       * \param velocity_ms - скорость точки в метрах в секунду. <i>По умолчанию равна нулю.</i>
       */
      explicit RoutePoint(const QGeoCoordinate& coord, float velocity_ms = 0);

      [[nodiscard]] QGeoCoordinate coordinate() const;      ///< Возвращает координату точки в виде QGeoCoordinate.
      void setCoordinate(const QGeoCoordinate&);            ///< Задает координату точки в виде QGeoCoordinate.

      [[nodiscard]] double latitude() const;                ///< Возвращает широту точки.
      void setLatitude(double);                             ///< Задает широту точки.

      [[nodiscard]] double longitude() const;               ///< Возвращает долготу точки.
      void setLongitude(double);                            ///< Задает долготу точки.

      [[nodiscard]] float altitude() const;                 ///< Возвращает высоту точки в метрах.
      void setAltitude(float);                              ///< Задает высоту точки в метрах.

      [[nodiscard]] float velocity() const;                 ///< Возвращает скорость точки в м/с.
      void setVelocity(float);                              ///< Задает скорость точки в м/с.

    private:
      QGeoCoordinate m_coordinate;
      float m_velocity;
  };

  inline RoutePoint::RoutePoint()
    : m_coordinate({0, 0, 0})
    , m_velocity(0)
  {}

  inline RoutePoint::RoutePoint(double latitude, double longitude, float elevation, float velocity_ms)
    : m_coordinate(latitude, longitude, elevation)
    , m_velocity(velocity_ms)
  {}

  inline RoutePoint::RoutePoint(const QGeoCoordinate& coord, float velocity_ms)
    : m_coordinate(coord)
    , m_velocity(velocity_ms)
  {}

  inline QGeoCoordinate RoutePoint::coordinate() const { return m_coordinate; }
  inline void RoutePoint::setCoordinate(const QGeoCoordinate& x) { m_coordinate = x; }
  inline double RoutePoint::latitude() const { return m_coordinate.latitude(); }
  inline void RoutePoint::setLatitude(double x) { m_coordinate.setLatitude(x); }
  inline double RoutePoint::longitude() const { return m_coordinate.longitude(); }
  inline void RoutePoint::setLongitude(double x) { m_coordinate.setLongitude(x); }
  inline float RoutePoint::altitude() const { return static_cast<float>(m_coordinate.altitude()); }
  inline void RoutePoint::setAltitude(float x) { m_coordinate.setAltitude(x); }
  inline float RoutePoint::velocity() const { return m_velocity; }
  inline void RoutePoint::setVelocity(float x) { if(not qIsNaN(x)) m_velocity = x; }
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::RoutePoint)