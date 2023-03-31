#pragma once

#include <QObject>
#include <QQuickItem>
#include <QSGGeometryNode>

namespace ChartsOpenGL {
    class CElevationWidgetPolyline : public QQuickItem
    {
        Q_OBJECT
        public:
            CElevationWidgetPolyline(QQuickItem* parent = nullptr);

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;
    };
} /// namespace ChartsOpenGL;
