#pragma once

#include <QtPositioning/QGeoCoordinate>

namespace ElevationChart
{
  /**
   * \property RoutePoint::coordinate
   * \brief Координата точки.
   * \details Представлена в виде QGeoCoordinate. Хранит широту, долготу и высоту в метрах.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>coordinate</i>  <td><i>setCoordinate</i><td><i>--</i>
   * </table>
   *
   * \property RoutePoint::latitude
   * \brief Широта точки.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>latitude</i>    <td><i>setLatitude</i>  <td><i>--</i>
   * </table>
   *
   * \property RoutePoint::longitude
   * \brief Широта точки.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>longitude</i>   <td><i>setLongitude</i> <td><i>--</i>
   * </table>
   *
   * \property RoutePoint::altitude
   * \brief Высота точки в метрах.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>altitude</i>    <td><i>setAltitude</i>  <td><i>--</i>
   * </table>
   *
   * \property RoutePoint::velocity
   * \brief Скорость в точке в метрах в секунду.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>velocity</i>    <td><i>setVelocity</i>  <td><i>--</i>
   * </table>
   */

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

      [[nodiscard]] QGeoCoordinate coordinate() const;
      void setCoordinate(const QGeoCoordinate&);

      [[nodiscard]] double latitude() const;
      void setLatitude(double);

      [[nodiscard]] double longitude() const;
      void setLongitude(double);

      [[nodiscard]] float altitude() const;
      void setAltitude(float);

      [[nodiscard]] float velocity() const;
      void setVelocity(float);

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