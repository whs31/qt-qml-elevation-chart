#pragma once

#include <QQuickItem>
#include <QSGGeometryNode>

namespace ChartsOpenGL {
    class CDeclarativePolyline : public QQuickItem
    {
        Q_OBJECT
        Q_PROPERTY(QString lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)

        QString m_lineColor = "#FF00DC"; // error color

        public:
            CDeclarativePolyline(QQuickItem* parent = nullptr);

            signals:
                void lineColorChanged();

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;

        private:
            QString lineColor() const;
            void setLineColor(const QString& col);
    };
} /// namespace ChartsOpenGL;

