#include "cdeclarativepolyline.hpp"
#include <cmath>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

using namespace ChartsOpenGL;

CDeclarativePolyline::CDeclarativePolyline(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    qDebug() << "<charts> CDeclarativePolyline initialized";
}

void CDeclarativePolyline::setList(const std::list<QPointF>& points)
{
    m_points = points;
    this->update();
}

QSGNode* CDeclarativePolyline::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    QSGGeometry* geometry = nullptr;
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);
    if(node == nullptr)
    {
        node = new QSGGeometryNode;
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(QColor(lineColor()));

        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_points.size(), 0, QSGGeometry::UnsignedIntType);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
    }


    geometry = node->geometry();
    geometry->allocate(m_points.size() + 1);

    geometry->setDrawingMode(GL_LINE_STRIP);
    geometry->setLineWidth(5);

    // это пиздец)
    // короче. если аллоцировать память под массив(лист) из N точек, то ничего работать не будет
    // поэтому мы аллоцируем память под N+1 точек.
    size_t index = 1;
    geometry->vertexDataAsPoint2D()[0].set(m_points.front().x(), m_points.front().y());
    for(QPointF point : m_points)
        geometry->vertexDataAsPoint2D()[index++].set(point.x(), point.y());
    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

QString CDeclarativePolyline::lineColor() const { return m_lineColor; }
void CDeclarativePolyline::setLineColor(const QString& col) {
    if (m_lineColor == col) return;
    m_lineColor = col;
    emit lineColorChanged();
}


