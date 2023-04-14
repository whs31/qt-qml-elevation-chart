#pragma once

#include <QQuickItem>
#include <QPointF>
#include <list>
#define __signal void

using std::list;

namespace ChartsOpenGL {
    class CDeclarativePolygon : public QQuickItem
    {
        Q_OBJECT
        Q_PROPERTY(QString fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
        Q_PROPERTY(float fl_LineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

        QString m_fillColor = "#FF00DC"; // error color
        list<QPointF> m_points;
        int m_loopmode = LoopMode::LoopByItemRect;
        float m_lineWidth = 1;

        public:
            enum LoopMode
            {
                LoopByItemRect,
                LoopByFirstAndLast,
                None
            };
            CDeclarativePolygon(QQuickItem* parent = nullptr);

            void setList(const list<QPointF>& points);
            void asyncAppend(const list<QPointF>& points);
            void clear();

            void setLoopMode(LoopMode mode);



        signals:
                __signal fillColorChanged();
                __signal drawingModeChanged();

                void lineWidthChanged();

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;

        private:
            QString fillColor() const;
            void setFillColor(const QString& col);

            float lineWidth() const;
            void setLineWidth(float other);
    };
} /// namespace ChartsOpenGL;

