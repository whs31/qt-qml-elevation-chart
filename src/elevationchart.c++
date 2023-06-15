#include "elevationchart.h"
#include <stdexcept>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <QtPositioning/QGeoPath>
#include <LPVL/GLGeometry>
#include <DEM/GroundProfile>

namespace Widgets
{

QGeoCoordinate ElevationChart::ChartPoint::toQGeoCoordinate() const noexcept
{
    return { latitude, longitude, altitude };
}

ElevationChart::ElevationChart(QQuickItem* parent)
    : LPVL::ChartBase{parent}
    , m_route({})
{
    setFlag(ItemHasContents);
}

void ElevationChart::setPolyline(const QList<QVariant>& route, const QList<float> velocities)
{
    m_route.clear();
    if(route.size() != velocities.size() and velocities.size() != 0)
        throw std::invalid_argument("Route list and Velocities list are different sizes");

    for(size_t i = 0; i < route.size(); ++i)
    {
        auto coord = route[i].value<QGeoCoordinate>();
        m_route.push_back({coord.latitude(), coord.longitude(), static_cast<float>(coord.altitude()), velocities[i]});
    }

    this->requestUpdate();
}

QList<QVariant> ElevationChart::getPolyline() const noexcept
{

}

QList<float> ElevationChart::getVelocities() const noexcept
{

}

void ElevationChart::applyMetricsCorrection() noexcept
{

}

void ElevationChart::estimateEnvelope() noexcept
{

}

void ElevationChart::applyEnvelopeCorrection() noexcept
{

}

QSGNode* ElevationChart::updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*)
{
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);

    if(node == nullptr)
    {
        node = new QSGGeometryNode;

        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(QColor(plottingColor()));
        QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0, QSGGeometry::UnsignedIntType);

        node->setMaterial(material);
        node->setGeometry(geometry);
        node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
    }

    node->removeAllChildNodes();

    if(drawBackground())
        node->appendChildNode(LPVL::scenegraph::createBackgroundNode(backgroundColor(), width(), height()));

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

void ElevationChart::requestUpdate()
{
    QGeoPath path;
    for(const auto& point : m_route)
        path.addCoordinate(point.toQGeoCoordinate());
    vector<DEM::GraphPoint> res = DEM::plotTerrainProfile(path);
}

QGeoCoordinate ElevationChart::uavPosition() const { return m_uavPosition; }
void ElevationChart::setUavPosition(const QGeoCoordinate& o) {
    if (m_uavPosition == o)
        return;
    m_uavPosition = o;
    emit uavPositionChanged();
}

bool ElevationChart::intersecting() const { return m_intersecting; }
void ElevationChart::setIntersecting(bool o) {
    if (m_intersecting == o)
        return;
    m_intersecting = o;
    emit intersectingChanged();
}

bool ElevationChart::valid() const { return m_valid; }
void ElevationChart::setValid(bool o) {
    if (m_valid == o)
        return;
    m_valid = o;
    emit validChanged();
}

float ElevationChart::climbRate() const { return m_climbRate; }
void ElevationChart::setClimbRate(float o) {
    if (qFuzzyCompare(m_climbRate, o))
        return;
    m_climbRate = o;
    emit climbRateChanged();
}

float ElevationChart::descendRate() const { return m_descendRate; }
void ElevationChart::setDescendRate(float o) {
    if (qFuzzyCompare(m_descendRate, o))
        return;
    m_descendRate = o;
    emit descendRateChanged();
}

float ElevationChart::fallbackVelocity() const { return m_fallbackVelocity; }
void ElevationChart::setFallbackVelocity(float o) {
    if (qFuzzyCompare(m_fallbackVelocity, o))
        return;
    m_fallbackVelocity = o;
    emit fallbackVelocityChanged();
}

bool ElevationChart::matchingMetrics() const { return m_matchingMetrics; }
void ElevationChart::setMatchingMetrics(bool o) {
    if (m_matchingMetrics == o)
        return;
    m_matchingMetrics = o;
    emit matchingMetricsChanged();
}

float ElevationChart::envelopeMinAltitude() const { return m_envelopeMinAltitude; }
void ElevationChart::setEnvelopeMinAltitude(float o) {
    if (qFuzzyCompare(m_envelopeMinAltitude, o))
        return;
    m_envelopeMinAltitude = o;
    emit envelopeMinAltitudeChanged();
}

float ElevationChart::envelopeCoridorSize() const { return m_envelopeCoridorSize; }
void ElevationChart::setEnvelopeCoridorSize(float o) {
    if (qFuzzyCompare(m_envelopeCoridorSize, o))
        return;
    m_envelopeCoridorSize = o;
    emit envelopeCoridorSizeChanged();
}

} // Widgets
