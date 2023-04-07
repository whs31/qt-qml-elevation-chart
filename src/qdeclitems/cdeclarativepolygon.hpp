#pragma once

#include <QQuickItem>
#include <QPointF>
#include <list>

using std::list;

namespace ChartsOpenGL {
    class CDeclarativePolygon : public QQuickItem
    {
        Q_OBJECT
        Q_PROPERTY(QString fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
        Q_PROPERTY(QString drawingMode READ drawingMode WRITE setDrawingMode NOTIFY drawingModeChanged)

        QString m_fillColor = "#FF00DC"; // error color
        list<QPointF> m_points;
        int m_loopmode = LoopMode::LoopByItemRect;
        QString m_drawingMode;
        uint32_t m_glDrawMode = 0x000;

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

            QString drawingMode() const;
            void setDrawingMode(const QString& newDrawingMode);

            signals:
                void fillColorChanged();
                void drawingModeChanged();

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;

        private:
            QString fillColor() const;
            void setFillColor(const QString& col);
    };
} /// namespace ChartsOpenGL;

