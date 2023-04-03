#include "cdeclarativepolyline.hpp"
#include <cmath>
#include <QSGFlatColorMaterial>

using namespace ChartsOpenGL;

CDeclarativePolyline::CDeclarativePolyline(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    qDebug() << "<charts> CDeclarativePolyline initialized";
}

QSGNode* CDeclarativePolyline::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);
    if(node == nullptr)
    {
        node = new QSGGeometryNode;
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(QColor(lineColor()));

        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
    geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
    geometry->setLineWidth(4);

    geometry->vertexDataAsPoint2D()[0].set(0, height() - 10);
    geometry->vertexDataAsPoint2D()[1].set(0, height() / 2);
    geometry->vertexDataAsPoint2D()[2].set(width(), height());
    geometry->vertexDataAsPoint2D()[3].set(width(), height() - 10);

    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    return node;
}

QString CDeclarativePolyline::lineColor() const { return m_lineColor; }
void CDeclarativePolyline::setLineColor(const QString& col) {
    if (m_lineColor == col) return;
    m_lineColor = col;
    emit lineColorChanged();
}

