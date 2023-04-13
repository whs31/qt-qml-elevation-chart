#pragma once
#include <QGeoCoordinate>

/// @namespace Пространство имен для библиотеки построения графиков.
namespace GLCharts
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

            //! @brief Возвращает True, если эта точка находится внутри валидной карты высот.
            //!        Возвращает False, если для этой точки не существует тайла карт высот.
            bool isValidElevationTilePoint();

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
} // namespace Charts;
