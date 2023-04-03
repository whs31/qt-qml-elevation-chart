#include "charts/elevationwidget.hpp"
#include "elevationwidget_p.hpp"
#include "Elevation/elevation.h"
#include "RouteTools/elevationtools.h"
#include "qdeclitems/cdeclarativepolyline.hpp"

#include <qqml.h>
#include <cmath>
#include <QMetaType>
#include <QQuickItem>
#include <QGeoPath>
#include <QVector>

using namespace Charts;

#pragma region PUBLIC

ElevationWidget::ElevationWidget(QObject *parent)
    : QObject{parent}
    , d_ptr(new ElevationWidgetPrivate(this))
{
    //qmlRegisterSingletonInstance("ElevationWidgetImpl", 1, 0, "Impl", d_ptr);

    #ifndef CUSTOM_DEBUG_OUTPUT
    #define CUSTOM_DEBUG_OUTPUT
    qSetMessagePattern("[%{time process}] [%{category}] "
                       "%{if-debug}\033[01;38;05;15m%{endif}"
                       "%{if-info}\033[01;38;05;146m%{endif}"
                       "%{if-warning}\033[1;33m%{endif}"
                       "%{if-critical}\033[1;31m%{endif}"
                       "%{if-fatal}F%\033[1;31m%{endif}%{message}\033[0m");
    #endif

    initialize_qrc_file_within_namespace_1("charts");
}

void ElevationWidget::linkWithQML(QQuickItem* rootObject)
{
    Q_D(ElevationWidget);
    d->linkWithQML(rootObject);
}

list<GeoPoint> ElevationWidget::getRoute()
{
    Q_D(ElevationWidget);
    return d->getRoute();
}

void ElevationWidget::setRoute(const std::list<GeoPoint>& route)
{
    Q_D(ElevationWidget);
    d->setRoute(route);
}

void ElevationWidget::setUAVPosition(const QGeoCoordinate& position)
{
    Q_D(ElevationWidget);
    d->setUAVPosition(position);
}

void ElevationWidget::setUAVPosition(double latitude, double longitude)
{
    Q_D(ElevationWidget);
    d->setUAVPosition(latitude, longitude);
}

bool ElevationWidget::isIntersecting()
{
    Q_D(ElevationWidget);
    return d->isIntersecting();
}

bool ElevationWidget::isValid()
{
    Q_D(ElevationWidget);
    return d->isValid();
}

void ElevationWidget::setClimbRate(float rate)
{
    Q_D(ElevationWidget);
    d->setClimbRate(rate);
}

void ElevationWidget::setDescendRate(float rate)
{
    Q_D(ElevationWidget);
    d->setDescendRate(rate);
}

void ElevationWidget::setGlobalVelocity(float velocity)
{
    Q_D(ElevationWidget);
    d->setGlobalVelocity(velocity);
}

void ElevationWidget::applyMetricsCorrection()
{
    Q_D(ElevationWidget);
    d->applyMetricsCorrection();
}

bool ElevationWidget::isMatchingMetrics()
{
    Q_D(ElevationWidget);
    return d->isMatchingMetrics();
}

void ElevationWidget::setEnvelopeMinimumAltitude(float altitude)
{
    Q_D(ElevationWidget);
    d->setEnvelopeMinimumAltitude(altitude);
}

void ElevationWidget::setEnvelopeCoridorSize(float distance)
{
    Q_D(ElevationWidget);
    d->setEnvelopeCoridorSize(distance);
}

void ElevationWidget::estimateEnvelope()
{
    Q_D(ElevationWidget);
    d->estimateEnvelope();
}

void ElevationWidget::applyEnvelopeCorrection()
{
    Q_D(ElevationWidget);
    d->applyEnvelopeCorrection();
}

#pragma endregion PUBLIC

#pragma region PRIVATE

ElevationWidgetPrivate::ElevationWidgetPrivate(ElevationWidget* parent)
    : QObject{parent}
    , q_ptr(parent)
{
    heightmapParser = new Elevation::Elevation(this);
    routeParser = new Elevation::ElevationTools(this);

    qRegisterMetaType<QVector<Elevation::Point>>("QVector<Point>");
    qRegisterMetaType<Elevation::RouteAndElevationProfiles>("RouteAndElevationProfiles");

    qmlRegisterType<ChartsOpenGL::CDeclarativePolyline>("CDeclarativePolyline", 1, 0, "CDeclarativePolyline");
}

void ElevationWidgetPrivate::linkWithQML(QQuickItem* rootObject)
{
    m_pathPolyline = rootObject->findChild<ChartsOpenGL::CDeclarativePolyline*>("qml_gl_path_polyline");

    if(not m_pathPolyline)
        qCritical() << "<charts> Failed to link with QML.";
    else
        qInfo() << "<charts> Path polyline linked successfully";
}

list<GeoPoint> ElevationWidgetPrivate::getRoute()
{
    return m_route;
}

void ElevationWidgetPrivate::setRoute(const std::list<GeoPoint>& route)
{
    m_route = route;
    if(not route.empty() and m_pathPolyline != nullptr)
        this->update(UpdateMode::RebuildProfile);
}

void ElevationWidgetPrivate::setUAVPosition(const QGeoCoordinate& position)
{
    if(position == m_uavPosition)
        return;
    m_uavPosition = position;
    // velocity xd
}

void ElevationWidgetPrivate::setUAVPosition(double latitude, double longitude)
{
    if(m_uavPosition == QGeoCoordinate(latitude, longitude))
        return;
    m_uavPosition = QGeoCoordinate(latitude, longitude);
    // velocity xd
}

bool ElevationWidgetPrivate::isIntersecting()
{

}

bool ElevationWidgetPrivate::isValid()
{

}

void ElevationWidgetPrivate::setClimbRate(float rate)
{

}

void ElevationWidgetPrivate::setDescendRate(float rate)
{

}

void ElevationWidgetPrivate::setGlobalVelocity(float velocity)
{

}

void ElevationWidgetPrivate::applyMetricsCorrection()
{

}

bool ElevationWidgetPrivate::isMatchingMetrics()
{

}

void ElevationWidgetPrivate::setEnvelopeMinimumAltitude(float altitude)
{

}

void ElevationWidgetPrivate::setEnvelopeCoridorSize(float distance)
{

}

void ElevationWidgetPrivate::estimateEnvelope()
{

}

void ElevationWidgetPrivate::applyEnvelopeCorrection()
{

}

void ElevationWidgetPrivate::update(UpdateMode mode)
{
    if(mode == UpdateMode::RebuildProfile)
    {
        QGeoPath path_to_build;
        for(GeoPoint point : m_route)
            path_to_build.addCoordinate(point.coordinate());
        QVector<QPointF> profile = heightmapParser->buildGroundProfileForChart(path_to_build);

        axis.x.maxValue = profile.last().x();
        axis.x.roundMaxValue = 0;
        axis.y.maxValue = 0;
        axis.y.roundMaxValue = 0;

        for(const QPointF& point_to_check_for_elevation_files : profile)
        {
            if(point_to_check_for_elevation_files.y() > __INT16_MAX__ - 1'000)
            {
                qWarning() << "<charts> Some elevation profiles are missing from /elevations folder.";
                //set file integrity
                return;
            }
        }
        for(const QGeoCoordinate& coordinate : path_to_build.path())
            if(coordinate.altitude() > axis.y.maxValue)
                axis.y.maxValue = coordinate.altitude();
        int power_of_x = axis.x.maxValue > 0 ? (int) log10 ((float) axis.x.maxValue) : 1;
        int power_of_y = axis.y.maxValue > 0 ? (int) log10 ((float) axis.y.maxValue) : 1;
        while(axis.x.roundMaxValue < axis.x.maxValue)
            axis.x.roundMaxValue += pow(10, power_of_x);
        while(axis.y.roundMaxValue < axis.y.maxValue)
            axis.y.roundMaxValue += pow(10, power_of_y);
        axis.x.scaleValue = pow(10, power_of_x);
        axis.y.scaleValue = pow(10, power_of_y);
        axis.x.scaleCount = (float)axis.x.maxValue / (float)axis.x.scaleValue;
        axis.y.scaleCount = (float)axis.y.maxValue * axis.stretch / (float)axis.y.scaleValue;
        axis.x.scalePixelSize = m_pathPolyline->width() / axis.x.scaleCount;
        axis.y.scalePixelSize = m_pathPolyline->height() / axis.y.scaleCount;

        list<QPointF> profile_polygon;
        for(size_t i = 0; i < profile.size(); ++i)
            profile_polygon.push_back(toPixelCoords(profile.at(i), axis.x.maxValue, axis.y.maxValue,
                                                    axis.stretch, m_pathPolyline->width(), m_pathPolyline->height()));
        // set profile here
    }

    list<QPointF> path_polyline;
    QGeoCoordinate prev_coord = m_route.front().coordinate();
    float distance_cap = 0;
    for(GeoPoint& point : m_route)
    {
        distance_cap += point.coordinate().distanceTo(prev_coord);
        QPointF distance_height_point = QPointF(distance_cap, point.altitude());
        path_polyline.push_back(toPixelCoords(distance_height_point, axis.x.maxValue, axis.y.maxValue,
                                              axis.stretch, m_pathPolyline->width(), m_pathPolyline->height()));
        prev_coord = point.coordinate();
    }
    m_pathPolyline->setList(path_polyline);
}

QPointF ElevationWidgetPrivate::toPixelCoords(const QPointF& point, float x_max, float y_max, float y_stretch, float pixel_width, float pixel_height)
{
    return QPointF(point.x() * pixel_width / x_max, pixel_height - point.y() * pixel_height / (y_max * y_stretch));
}

#pragma endregion PRIVATE
