#include "velocitypoint.h"
#include <QtQml/qqml.h>

namespace Widgets
{

__attribute__((constructor))
static void register_self() {
    qmlRegisterType<VelocityPoint>("Widgets.impl", 1, 0, "VelocityPoint"); qInfo() << "<Widgets> Registered type VelocityPoint";
}

VelocityPoint::VelocityPoint(QObject *parent)
    : QObject{parent}
{
    setLatitude(0);
    setLongitude(0);
    setAltitude(0);
    setVelocity(0);
}

VelocityPoint::VelocityPoint(double lat, double lon, float alt, float vel, QObject* parent)
    : QObject{parent}
{
    setLatitude(lat);
    setLongitude(lon);
    setAltitude(alt);
    setVelocity(vel);
}

double VelocityPoint::latitude() const { return m_latitude; }
void VelocityPoint::setLatitude(double other) {
    if (qFuzzyCompare(m_latitude, other))
        return;
    m_latitude = other;
    emit latitudeChanged();
}

double VelocityPoint::longitude() const { return m_longitude; }
void VelocityPoint::setLongitude(double other) {
    if (qFuzzyCompare(m_longitude, other))
        return;
    m_longitude = other;
    emit longitudeChanged();
}

float VelocityPoint::altitude() const { return m_altitude; }

void VelocityPoint::setAltitude(float other) {
    if (qFuzzyCompare(m_altitude, other))
        return;
    m_altitude = other;
    emit altitudeChanged();
}

float VelocityPoint::velocity() const { return m_velocity; }
void VelocityPoint::setVelocity(float other) {
    if (qFuzzyCompare(m_velocity, other))
        return;
    m_velocity = other;
    emit velocityChanged();
}

} // Widgets
