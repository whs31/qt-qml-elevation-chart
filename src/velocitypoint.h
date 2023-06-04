#pragma once

#include <QtCore/QObject>

namespace Widgets
{
    class VelocityPoint : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
        Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
        Q_PROPERTY(float altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
        Q_PROPERTY(float velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)

        public:
            VelocityPoint(QObject* parent = nullptr);
            VelocityPoint(double lat, double lon, float alt, float vel = 0, QObject* parent = nullptr);
            double latitude() const; void setLatitude(double);
            double longitude() const; void setLongitude(double);
            float altitude() const; void setAltitude(float);
            float velocity() const; void setVelocity(float);

            signals:
                void latitudeChanged();
                void longitudeChanged();
                void altitudeChanged();
                void velocityChanged();

        private:
            double m_latitude;
            double m_longitude;
            float m_altitude;
            float m_velocity;
    };
} // Widgets
