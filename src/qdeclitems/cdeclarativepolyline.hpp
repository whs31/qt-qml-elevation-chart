#pragma once

#include <QQuickItem>
#include <QPointF>
#include <list>
#define __signal void

using std::list;
using std::vector;

namespace ChartsOpenGL {
    class CDeclarativePolyline : public QQuickItem
    {
        Q_OBJECT
        Q_PROPERTY(QString lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)

        QString m_lineColor = "#FF00DC"; // error color
        list<QPointF> m_points;

        public:
            enum DashPattern
            {
                Dashed,
                Dotted
            };
            Q_ENUMS(DashPattern)

            CDeclarativePolyline(QQuickItem* parent = nullptr);

            void setList(const list<QPointF>& points);
            void clear();

            QString lineColor() const;
            void setLineColor(const QString& col);

            signals:
                __signal lineColorChanged();

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;
    };
} // namespace ChartsOpenGL;

