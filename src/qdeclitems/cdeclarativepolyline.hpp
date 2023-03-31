#pragma once

#include <QObject>
#include <QQuickItem>
#include <QSGGeometryNode>

class CDeclarativePolyline : public QQuickItem
{
    Q_OBJECT
    QSGGeometryNode* m_borderActiveNode;

    public:
        CDeclarativePolyline(QQuickItem* parent = nullptr);

    protected:
        virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;

    signals:

};

