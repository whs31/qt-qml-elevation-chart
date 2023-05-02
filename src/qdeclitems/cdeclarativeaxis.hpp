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
        Q_PROPERTY(QString fontfamily READ fontfamily WRITE setFontfamily NOTIFY fontfamilyChanged)
        Q_PROPERTY(qreal oxScrollPosition READ oxScrollPosition WRITE setOXScrollPosiition NOTIFY oxScrollPositionChanged)
        Q_PROPERTY(qreal oxScrollSize READ oxScrollSize WRITE setOXScrollSize NOTIFY oxScrollSizeChanged)

        QString m_color = "#FF0000";
        QVector4D m_offsets;
        QString m_fontfamily;
        qreal m_oxScrollPosition = 0;
        qreal m_oxScrollSize = 1;

        struct Axis
        {
            float max = 0;
            float min = 0;
            float round_max;
            int scale_value;
            float scale_count;
            float scale_pixel_size;
            float stretch = 1.2;
            float offset = 0;
            float pixel_offset = 0;
        } axis_x, axis_y, axis_rely;

        public:
            CDeclarativeAxis(QQuickItem* parent = nullptr);

            void paint(QPainter* painter) override;
            void set(float stretch, float xmax, float ymax, float yreloffset, float xmin = 0, float ymin = 0);

        signals:
                __signal colorChanged();
                __signal offsetsChanged();
                __signal fontfamilyChanged();
                __signal oxScrollPositionChanged();

                void oxScrollSizeChanged();

        private:
            QString color() const;
            void setColor(const QString& col);
            QVector4D offsets() const;
            void setOffsets(const QVector4D& other);
            QString fontfamily() const;
            void setFontfamily(const QString& other);
            qreal oxScrollPosition() const;
            void setOXScrollPosiition(qreal other);
            qreal oxScrollSize() const;
            void setOXScrollSize(qreal other);
    };
}
