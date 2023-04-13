#pragma once

#include <QQuickItem>
#include <QPointF>
#include <list>
#define __signal void

using std::list;

namespace ChartsOpenGL {
    class CDeclarativeMultipolygon : public QQuickItem
    {
        Q_OBJECT
        Q_PROPERTY(QString fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)

        QString m_fillColor = "#FF00DC"; // error color
        list<QPointF> m_points;

        public:
            CDeclarativeMultipolygon(QQuickItem* parent = nullptr);

            void setList(const list<QPointF>& points);
            void asyncAppend(const list<QPointF>& points);
            void clear();

            signals:
                __signal fillColorChanged();

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;

        private:
            QString fillColor() const;
            void setFillColor(const QString& col);
    };
} // namespace ChartsOpenGL;

