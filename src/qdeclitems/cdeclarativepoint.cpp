#include "cdeclarativepoint.hpp"
//#include "../scenegraph/glpointshader.hpp"
#include "../scenegraph/glvertextype.hpp"

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <cmath>

using namespace ChartsOpenGL;

CDeclarativePoint::CDeclarativePoint(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
}

QSGNode* CDeclarativePoint::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    QSGGeometry* geometry = nullptr;
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);

    if(node == nullptr)
    {
        node = new QSGGeometryNode;
        //QSGSimpleMaterial<State>* material = GLPointShader::createMaterial();
        //material->setFlag(QSGMaterial::Blending);
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(color());

        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        //static_cast<QSGSimpleMaterial<State>*>(node->material())->state()->color = QColor(m_color);
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 90, 0, QSGGeometry::UnsignedIntType);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
    }

    geometry = node->geometry();
    geometry->setDrawingMode(GL_POLYGON);
    geometry->setLineWidth(1);

    geometry->allocate(90);


    for(size_t i = 0; i < 90; ++i)
    {
        float rad = i * M_PI / 180 * 4;
        geometry->vertexDataAsPoint2D()[i].set(cos(rad) * radius() + width() / 2,
                                                       sin(rad) * radius() + height() / 2);
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}


QString CDeclarativePoint::color() const { return m_color; }
void CDeclarativePoint::setColor(const QString& col) {
    if (m_color == col) return;
    m_color = col;
    emit colorChanged();
}

float CDeclarativePoint::radius() const { return m_radius; }
void CDeclarativePoint::setRadius(float val) {
    if (qFuzzyCompare(m_radius, val)) return;
    m_radius = val;
    emit radiusChanged();
}
