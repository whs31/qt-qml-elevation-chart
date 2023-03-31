#include "cdeclarativepolyline.hpp"
#include <cmath>
#include <QSGFlatColorMaterial>

CDeclarativePolyline::CDeclarativePolyline(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    qDebug() << "<charts> CDeclarativePolyline initialized";

    //connect(this, &CDeclarativePolyline::widthChanged, this, &CDeclarativePolyline::update);
}

QSGNode* CDeclarativePolyline::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);
    if(node == nullptr)
    {
        node = new QSGGeometryNode;
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(Qt::yellow);

        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
    geometry->setDrawingMode(GL_LINE_LOOP);

    geometry->vertexDataAsPoint2D()[0].set(0, 0);
    geometry->vertexDataAsPoint2D()[1].set(0, height());
    geometry->vertexDataAsPoint2D()[2].set(width(), height());
    geometry->vertexDataAsPoint2D()[3].set(width(), 0);

    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    return node;
}
