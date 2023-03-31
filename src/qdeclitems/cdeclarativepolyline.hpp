#pragma once

#include <QQuickItem>
#include <QSGGeometryNode>

namespace ChartsOpenGL {
    class CDeclarativePolyline : public QQuickItem
    {
        Q_OBJECT
        public:
            CDeclarativePolyline(QQuickItem* parent = nullptr);

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;
    };
} /// namespace ChartsOpenGL;

