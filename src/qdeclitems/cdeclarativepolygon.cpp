#include "cdeclarativepolygon.hpp"
#include <cmath>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

using namespace ChartsOpenGL;

CDeclarativePolygon::CDeclarativePolygon(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    qDebug() << "<charts> CDeclarativePolygon initialized";
}

void CDeclarativePolygon::setList(const std::list<QPointF>& points)
{
    m_points = points;
    this->update();
}

QSGNode* CDeclarativePolygon::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    QSGGeometry* geometry = nullptr;
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);
    if(node == nullptr)
    {
        node = new QSGGeometryNode;
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(QColor(fillColor()));

        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_points.size(), 0, QSGGeometry::UnsignedIntType);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
    }

    geometry = node->geometry();                                                          
    geometry->allocate(m_loopmode == LoopMode::LoopByItemRect ? m_points.size() * 2 + 2 : m_points.size() * 2 + 2);
    //                                                                                ^
    //                             3 вершины нужны чтобы достроить по boundingRect()  | 03.04: временно отключил

    geometry->setDrawingMode(GL_QUAD_STRIP);
    geometry->setLineWidth(1);

    // это пиздец)
    // короче. если аллоцировать память под массив (лист) из N точек, то ничего работать не будет
    // поэтому мы аллоцируем память под N+1 точек и присваиваем нулевой точке значение первой
    size_t index = 1;
    geometry->vertexDataAsPoint2D()[0].set(m_points.front().x(), m_points.front().y());
    for(QPointF point : m_points)
    {
        geometry->vertexDataAsPoint2D()[index++].set(point.x(), point.y());
        geometry->vertexDataAsPoint2D()[index++].set(point.x(), height());
    }
    geometry->vertexDataAsPoint2D()[m_points.size() * 2 + 1].set(m_points.back().x(), height());

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

QString CDeclarativePolygon::fillColor() const { return m_fillColor; }
void CDeclarativePolygon::setFillColor(const QString& col) {
    if (m_fillColor == col) return;
    m_fillColor = col;
    emit fillColorChanged();
}

void ChartsOpenGL::CDeclarativePolygon::setLoopMode(LoopMode mode)
{
    m_loopmode = mode;
}
