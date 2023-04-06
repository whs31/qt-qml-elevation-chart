#pragma once

#include <QQuickItem>
#include <QPointF>

namespace ChartsOpenGL {
    class CDeclarativePoint : public QQuickItem
    {
        Q_OBJECT
        Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)

        QString m_color = "#FF00DC"; // error color
        float m_radius = 10;

        public:
            CDeclarativePoint(QQuickItem* parent = nullptr);

            QString color() const;
            void setColor(const QString& col);

            float radius() const;
            void setRadius(float val);

            signals:
                void colorChanged();
                void radiusChanged();

        protected:
                virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;
    };
}

