#pragma once
#include <QQuickPaintedItem>

namespace ChartsOpenGL {
    class CDeclarativeAxis : public QQuickPaintedItem
    {
        Q_OBJECT
        Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

        QString m_color = "#FF0000";
        float m_max = 0;
        float m_min = 0;

        public:
            CDeclarativeAxis(QQuickItem* parent = nullptr);

            void paint(QPainter* painter) override;
            void set(float max, float min = 0);

            signals:
                void colorChanged();
        private:
            QString color() const;
            void setColor(const QString& col);
    };
}
