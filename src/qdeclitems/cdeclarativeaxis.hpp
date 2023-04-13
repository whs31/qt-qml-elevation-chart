#pragma once
#include <QQuickPaintedItem>
#include <QVector4D>
#define __signal void

namespace ChartsOpenGL {
    class CDeclarativeAxis : public QQuickPaintedItem
    {
        Q_OBJECT
        Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(QVector4D offsets READ offsets WRITE setOffsets NOTIFY offsetsChanged)

        QString m_color = "#FF0000";
        QVector4D m_offsets;
        float m_max = 0;
        float m_min = 0;

        public:
            CDeclarativeAxis(QQuickItem* parent = nullptr);

            void paint(QPainter* painter) override;
            void set(float max, float min = 0);

            signals:
                __signal colorChanged();
                __signal offsetsChanged();

        private:
            QString color() const;
            void setColor(const QString& col);

            QVector4D offsets() const;
            void setOffsets(const QVector4D& other);
    };
}
