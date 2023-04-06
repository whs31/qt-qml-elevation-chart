#include "charts/elevationwidget.hpp"
#include "elevationwidget_p.hpp"
#include "Elevation/elevation.h"
#include "RouteTools/elevationtools.h"
#include "pointsmodel.hpp"
#include "qdeclitems/cdeclarativepolyline.hpp"
#include "qdeclitems/cdeclarativepolygon.hpp"

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
    , model(new PointsModel(this))
    , heightmapParser(new Elevation::Elevation(this))
    , routeParser(new Elevation::ElevationTools(this))
{
    qRegisterMetaType<QVector<Elevation::Point>>("QVector<Point>");
    qRegisterMetaType<Elevation::RouteAndElevationProfiles>("RouteAndElevationProfiles");

    qmlRegisterType<ChartsOpenGL::CDeclarativePolyline>("CDeclarativePolyline", 1, 0, "CDeclarativePolyline");
    qmlRegisterType<ChartsOpenGL::CDeclarativePolygon>("CDeclarativePolygon", 1, 0, "CDeclarativePolygon");
    qmlRegisterSingletonInstance<PointsModel>("PointModel", 1, 0, "PointModel", model);

    connect(heightmapParser, &Elevation::Elevation::profileAsyncNotification, this, [](unsigned int return_code){
        qDebug() << "<charts> Received notification from async calc:" << Qt::hex << return_code << Qt::dec;
        if(return_code == 0xFF)
            qWarning() << "<charts> Some elevation profiles are missing from /elevations folder.";
    });

    connect(heightmapParser, &Elevation::Elevation::profileAsyncPacket, this, &ElevationWidgetPrivate::sync);
    connect(routeParser, &Elevation::ElevationTools::progressBuildRouteAndElevationProfiles, this,
            &ElevationWidgetPrivate::calculateEnvelopeFinished);
    connect(routeParser, &Elevation::ElevationTools::progressTestRouteIntersectGround, this,
            &ElevationWidgetPrivate::calculateIntersectsFinished);
}

void ElevationWidgetPrivate::linkWithQML(QQuickItem* rootObject)
{
    m_pathPolyline = rootObject->findChild<ChartsOpenGL::CDeclarativePolyline*>("qml_gl_path_polyline");
    m_metricsPolyline = rootObject->findChild<ChartsOpenGL::CDeclarativePolyline*>("qml_gl_metrics_polyline");
    m_envelopePolyline = rootObject->findChild<ChartsOpenGL::CDeclarativePolyline*>("qml_gl_envelope_polyline");
    m_profilePolygon = rootObject->findChild<ChartsOpenGL::CDeclarativePolygon*>("qml_gl_profile_polygon");

    if(not m_pathPolyline)
        qCritical() << "<charts> Failed to link with QML at qml_gl_path_polyline";
    else
        qInfo() << "<charts> qml_gl_path_polyline linked successfully";

    if(not m_metricsPolyline)
        qCritical() << "<charts> Failed to link with QML at qml_gl_metrics_polyline";
    else
        qInfo() << "<charts> qml_gl_metrics_polyline linked successfully";

    if(not m_envelopePolyline)
        qCritical() << "<charts> Failed to link with QML at qml_gl_envelope_polyline";
    else
        qInfo() << "<charts> qml_gl_envelope_polyline linked successfully";

    if(not m_profilePolygon)
        qCritical() << "<charts> Failed to link with QML at qml_gl_profile_polygon";
    else
        qInfo() << "<charts> qml_gl_profile_polygon linked successfully";
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
    axis.relative_height = heightmapParser->elevation(m_uavPosition.latitude(), m_uavPosition.longitude());
    qInfo() << "<charts> Using UAV relative height" << axis.relative_height << "meters";
    update(UpdateMode::RebuildProfile);
    // velocity xd
}

void ElevationWidgetPrivate::setUAVPosition(double latitude, double longitude)
{
    if(m_uavPosition == QGeoCoordinate(latitude, longitude))
        return;
    m_uavPosition = QGeoCoordinate(latitude, longitude);
    axis.relative_height = heightmapParser->elevation(m_uavPosition.latitude(), m_uavPosition.longitude());
    qInfo() << "<charts> Using UAV relative height" << axis.relative_height << "meters";
    update(UpdateMode::RebuildProfile);
    // velocity xd
}

bool ElevationWidgetPrivate::isIntersecting()
{
    return m_intersects;
}

bool ElevationWidgetPrivate::isValid()
{
    return m_valid;
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
    this->aircraftMetrics.envelopeHeight = altitude;
}

void ElevationWidgetPrivate::setEnvelopeCoridorSize(float distance)
{
    this->aircraftMetrics.envelopeSize = distance;
}

void ElevationWidgetPrivate::estimateEnvelope()
{
    this->calculateEnvelope();
}

void ElevationWidgetPrivate::applyEnvelopeCorrection()
{
    setRoute(toRoute(m_envelope));
}

#pragma endregion PRIVATE

#pragma region IMPLEMENTATION

void ElevationWidgetPrivate::update(UpdateMode mode, float force_y_axis_height)
{
    if(mode == UpdateMode::RebuildProfile)
    {
        m_profilePolygon->clear();            // TODO: это нужно для асинхронной загрузки профиля по частям. пока выключил.
        QPointF bounds =  heightmapParser->buildProfileChartAsync(fromRoute(m_route));//, axis.relative_height);

        axis.x.maxValue = bounds.x();
        axis.x.roundMaxValue = 0;
        axis.y.maxValue = force_y_axis_height ? force_y_axis_height : bounds.y();
        axis.y.roundMaxValue = 0;

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
    m_envelopePolyline->clear();
}

void ElevationWidgetPrivate::sync(QVector<QPointF> vec)
{
    list<QPointF> packet;
    for(size_t i = 0; i < vec.size(); ++i)
    {
        packet.push_back(toPixelCoords(vec.at(i), axis.x.maxValue, axis.y.maxValue,
                         axis.stretch, m_pathPolyline->width(), m_pathPolyline->height()));
    }
    m_profilePolygon->asyncAppend(packet);
}

void ElevationWidgetPrivate::calculateEnvelope()
{
    if(m_route.empty())
    {
        qWarning() << "<charts> Route does not contain any points, envelope calculation will not be executed.";
        return;
    }
    QGeoPath path_to_process;
    for(GeoPoint point : m_route)
        path_to_process.addCoordinate(point.coordinate());
    routeParser->buildRouteAndElevationProfiles(path_to_process, aircraftMetrics.envelopeHeight, aircraftMetrics.envelopeSize,
                                                aircraftMetrics.velocity, aircraftMetrics.climbRate, aircraftMetrics.descendRate);
}

void ElevationWidgetPrivate::calculateEnvelopeFinished(quint8 progress, const Elevation::RouteAndElevationProfiles& deltaResult)
{
    QVector<Elevation::Point> _route = deltaResult.route();
    float _local_y_max = 0;
    m_envelope.clearPath();
    for(const Elevation::Point& point : _route)
    {
        QGeoCoordinate coord(point.latitude(), point.longitude(), point.altitude());
        m_envelope.addCoordinate(coord);
        if(coord.altitude() > _local_y_max)
            _local_y_max = coord.altitude();
    }
    update(UpdateMode::RebuildProfile, _local_y_max);

    list<QPointF> _list;
    float prev_distance = 0;
    for(size_t i = 0; i < m_envelope.path().size(); i++)
    {
        float ds = 0;
        if(i > 0)
            ds = m_envelope.path()[i].distanceTo(m_envelope.path()[i-1]);
        prev_distance += ds;
        QPointF point(prev_distance, m_envelope.path()[i].altitude()); //- axis.relative_height);
        _list.push_back(toPixelCoords(point, axis.x.maxValue, axis.y.maxValue, axis.stretch,
                                      m_envelopePolyline->width(), m_envelopePolyline->height()));
    }
    m_envelopePolyline->setList(_list);
}

void ElevationWidgetPrivate::calculateIntersects()
{

}

void ElevationWidgetPrivate::calculateIntersectsFinished(quint8 progress, const QVector<Elevation::Point>& resultPath)
{

}

QPointF ElevationWidgetPrivate::toPixelCoords(const QPointF& point, float x_max, float y_max, float y_stretch, float pixel_width, float pixel_height)
{
    return QPointF(point.x() * pixel_width / x_max, pixel_height - point.y() * pixel_height / (y_max * y_stretch));
}

list<GeoPoint> ElevationWidgetPrivate::toRoute(const QGeoPath& path)
{
    list<GeoPoint> ret;
    for(QGeoCoordinate coord : path.path())
        ret.push_back(GeoPoint(coord, aircraftMetrics.velocity));
    return ret;
}

QGeoPath ElevationWidgetPrivate::fromRoute(const std::list<GeoPoint> route)
{
    QGeoPath ret;
    for(GeoPoint point : route)
        ret.addCoordinate(point.coordinate());
    return ret;
}

#pragma endregion IMPLEMENTATION
