#include "cdeclarativepolyline.hpp"
#include <cmath>
#include <QSGFlatColorMaterial>

CDeclarativePolyline::CDeclarativePolyline(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    qDebug() << "c++";
}

QSGNode* CDeclarativePolyline::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    if(not old_node)
    {
        QSGGeometry* border_non_active_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 360);
        border_non_active_geometry->setDrawingMode(GL_POLYGON);


        for(size_t i = 0; i < 360; ++i)
        {
            auto rad = (i - 90) * M_PI / 180;
            border_non_active_geometry->vertexDataAsPoint2D()[i].set(cos(rad) * width(), sin(rad) * width() / 2 + height() / 2);
        }

        QSGFlatColorMaterial* border_non_active_material = new QSGFlatColorMaterial();
        border_non_active_material->setColor(Qt::red);

        QSGGeometryNode* border_non_active_node = new QSGGeometryNode();
        border_non_active_node->setGeometry(border_non_active_geometry);
        border_non_active_node->setMaterial(border_non_active_material);
        border_non_active_node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);

        return border_non_active_node;
    }
    else
    {

    }

    return old_node;
}
