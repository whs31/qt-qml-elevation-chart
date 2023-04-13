#include "cdeclarativepolyline.hpp"
#include "scenegraph/glvertextype.hpp"
#include "scenegraph/gllineshader.hpp"

#include <cmath>
#include <QSGGeometryNode>
#include <vector>

using namespace ChartsOpenGL;

CDeclarativePolyline::CDeclarativePolyline(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    #ifdef QT_DEBUG
        qDebug() << "<charts> CDeclarativePolyline initialized";
    #endif
}

void CDeclarativePolyline::setList(const std::list<QPointF>& points)
{
    m_points = points;
    this->update();
}

void CDeclarativePolyline::clear()
{
    m_points.clear();
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
        QSGSimpleMaterial<State>* material = GLLineShader::createMaterial();
        node->setMaterial(material);
        //material->setFlag(QSGMaterial::Blending);
        node->setFlag(QSGNode::OwnsMaterial);
        static_cast<QSGSimpleMaterial<State>*>(node->material())->state()->color = QColor(lineColor());

        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), m_points.size(), 0, QSGGeometry::UnsignedIntType);
        geometry->setDrawingMode(GL_LINE_STRIP);
        geometry->setLineWidth(5);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
    }

    geometry = node->geometry();
    vector<VertexT> glVec;
    for(QPointF point : m_points)
        glVec.push_back(VertexT(point.x(), point.y(), point.x() / width(), point.y() / height()));

    geometry->allocate(glVec.size());
    for(size_t i = 0; i < glVec.size(); i++)
        geometry->vertexDataAsTexturedPoint2D()[i].set(glVec.at(i).x, glVec.at(i).y, glVec.at(i).u, glVec.at(i).v);

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

QString CDeclarativePolyline::lineColor() const { return m_lineColor; }
void CDeclarativePolyline::setLineColor(const QString& col) {
    if (m_lineColor == col) return;
    m_lineColor = col;
    emit lineColorChanged();
}
