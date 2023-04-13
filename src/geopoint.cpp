#include "../include/charts/geopoint.hpp"

using namespace Charts;

GeoPoint::GeoPoint(const QGeoCoordinate &coordinate, float velocity)
    : m_coordinate(coordinate)
    , m_velocity(velocity) {}

GeoPoint::GeoPoint(double latitude, double longitude, float altitude, float velocity)
    : m_coordinate(QGeoCoordinate(latitude, longitude, altitude))
    , m_velocity(velocity) {}

GeoPoint::GeoPoint()
    : m_coordinate(QGeoCoordinate())
    , m_velocity(-1) {}

QGeoCoordinate GeoPoint::coordinate()
{
    return m_coordinate;
}

float GeoPoint::velocity()
{
    return m_velocity;
}

double GeoPoint::latitude()
{
    return m_coordinate.latitude();
}

double GeoPoint::longitude()
{
    return m_coordinate.longitude();
}

float GeoPoint::altitude()
{
    return m_coordinate.altitude();
}

bool GeoPoint::isValidElevationTilePoint()
{
    return m_coordinate.altitude() < __INT16_MAX__ - 1'000;
}

void GeoPoint::setCoordinate(const QGeoCoordinate& other)
{
    assert(other.isValid());
    m_coordinate = other;
}

void GeoPoint::setVelocity(float other)
{
    assert(other >= 0);
    m_velocity = other;
}

void GeoPoint::setLatitude(double other)
{
    assert(other <= 90 and other >= -90);
    m_coordinate.setLatitude(other);
}

void GeoPoint::setLongitude(double other)
{
    assert(other <= 180 and other >= -180);
    m_coordinate.setLongitude(other);
}

void GeoPoint::setAltitude(float other)
{
    m_coordinate.setAltitude(other);
}
