#pragma once
#include <QGeoCoordinate>

/// @namespace Пространство имен для библиотеки построения графиков.
namespace Charts
{
    //! @class Точка на земной поверхности с определенной скоростью.
    class GeoPoint
    {
        public:
            //! @brief Создает точку с заданными координатами и скоростью.
            //! @param coordinate - координаты точки в виде QGeoCoordinate;
            //! @param velocity - скорость точки (в м/с).
            GeoPoint(const QGeoCoordinate& coordinate, float velocity);

            //! @brief Создает точку с заданными координатами и скоростью.
            //! @param latitude - географическая широта точки;
            //! @param longitude - географическая долгота точки;
            //! @param altitude - высота точки относительно борта;
            //! @param velocity - скорость точки (в м/с).
            GeoPoint(double latitude, double longitude, float altitude, float velocity);

            //! @brief Создает невалидную точку с невалидными координатами и скоростью в -1 м/с.
            GeoPoint();

            //! @brief Возвращает геокоординаты точки.
            QGeoCoordinate coordinate();

            //! @brief Возвращает скорость точки в м/с.
            float velocity();

            //! @brief Возвращает широту точки.
            double latitude();

            //! @brief Возвращает долготу точки.
            double longitude();

            //! @brief Возвращает высоту точки относительно уровня борта.
            float altitude();

            //! @brief Задает координаты точки.
            void setCoordinate(const QGeoCoordinate& other);

            //! @brief Задает скорость точки в м/с.
            void setVelocity(float other);

            //! @brief Задает широту точки.
            void setLatitude(double other);

            //! @brief Задает долготу точки.
            void setLongitude(double other);

            //! @brief Задает высоту точки.
            void setAltitude(float other);

        private:
            QGeoCoordinate m_coordinate;
            float m_velocity;
    };

    inline GeoPoint::GeoPoint(const QGeoCoordinate& coordinate, float velocity)
            : m_coordinate(coordinate)
            , m_velocity(velocity) {}

    inline GeoPoint::GeoPoint(double latitude, double longitude, float altitude, float velocity)
        : m_coordinate(QGeoCoordinate(latitude, longitude, altitude))
        , m_velocity(velocity) {}

    inline GeoPoint::GeoPoint()
        : m_coordinate(QGeoCoordinate())
        , m_velocity(-1) {}

    inline QGeoCoordinate GeoPoint::coordinate()
    {
        return m_coordinate;
    }

    inline float GeoPoint::velocity()
    {
        return m_velocity;
    }

    inline double GeoPoint::latitude()
    {
        return m_coordinate.latitude();
    }

    inline double GeoPoint::longitude()
    {
        return m_coordinate.longitude();
    }

    inline float GeoPoint::altitude()
    {
        return m_coordinate.altitude();
    }

    inline void GeoPoint::setCoordinate(const QGeoCoordinate& other)
    {
        assert(other.isValid());
        m_coordinate = other;
    }

    inline void GeoPoint::setVelocity(float other)
    {
        assert(other >= 0);
        m_velocity = other;
    }

    inline void GeoPoint::setLatitude(double other)
    {
        assert(other <= 90 and other >= -90);
        m_coordinate.setLatitude(other);
    }

    inline void GeoPoint::setLongitude(double other)
    {
        assert(other <= 180 and other >= -180);
        m_coordinate.setLongitude(other);
    }

    inline void GeoPoint::setAltitude(float other)
    {
        assert(other < INT16_MAX);
        m_coordinate.setAltitude(other);
    }

} /// namespace Charts;
