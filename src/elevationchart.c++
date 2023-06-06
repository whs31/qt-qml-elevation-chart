#include "elevationchart.h"
#include <stdexcept>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <QtPositioning/QGeoCoordinate>
#include <QtPositioning/QGeoPath>
#include <LPVL/GLGeometry>
#include <DEM/GroundProfile>

namespace Widgets
{

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

QGeoCoordinate ElevationChart::ChartPoint::toQGeoCoordinate() const noexcept
{
    return { latitude, longitude, altitude };
}

} // Widgets
