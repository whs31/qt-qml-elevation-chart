#include "elevationchart.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <LPVL/GLGeometry>

namespace Widgets
{

ElevationChart::ElevationChart(QQuickItem* parent)
    : LPVL::ChartBase{parent}
{
    setFlag(ItemHasContents);
}

void ElevationChart::setPolyline(const QList<VelocityPoint>& route)
{
    qDebug() << route.front().longitude();
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

} // Widgets
