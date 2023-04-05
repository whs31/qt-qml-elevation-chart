#pragma once

#include <QQuickItem>
#include <QPointF>
#include <list>

using std::list;
using std::vector;

namespace ChartsOpenGL {
    class CDeclarativePolyline : public QQuickItem
    {
        Q_OBJECT
        Q_PROPERTY(QString lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
        Q_PROPERTY(bool dashed READ dashed WRITE setDashed NOTIFY dashedChanged)
        Q_PROPERTY(int dashPattern READ dashPattern WRITE setDashPattern NOTIFY dashPatternChanged)
        Q_PROPERTY(float dashInterval READ dashInterval WRITE setDashInterval NOTIFY dashIntervalChanged)

        QString m_lineColor = "#FF00DC"; // error color
        list<QPointF> m_points;
        bool m_dashed = false;
        float m_dashInterval = 15;
        int m_dashPattern = DashPattern::Dotted;

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

            bool dashed() const;
            void setDashed(bool state);

            QString lineColor() const;
            void setLineColor(const QString& col);

            float dashInterval() const;
            void setDashInterval(float interval);

            int dashPattern() const;
            void setDashPattern(const int& pattern);

            signals:
                void lineColorChanged();
                void dashedChanged();
                void dashIntervalChanged();
                void dashPatternChanged();

        protected:
            virtual QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData* update_paint_node_data) override;

        private:

    };
} /// namespace ChartsOpenGL;

