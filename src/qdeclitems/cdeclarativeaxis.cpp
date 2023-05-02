#include "cdeclarativeaxis.hpp"
#include <QPainter>
#include <QBrush>
#include <cmath>

using namespace ChartsOpenGL;

CDeclarativeAxis::CDeclarativeAxis(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{

}

void CDeclarativeAxis::paint(QPainter* painter)
{
    if(axis_x.max < 0)
        return;
    QColor col(m_color);
    QBrush brush(col);
    QPen pen(brush, 2, Qt::PenStyle::DashLine, Qt::PenCapStyle::SquareCap, Qt::PenJoinStyle::BevelJoin);
    QFont font;
    font.setPixelSize(11);
    font.setBold(true);
    painter->setFont(font);

    painter->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawLine(offsets().x(), height() - offsets().w(), offsets().x(), 0); // ORelY
    painter->drawLine(width() - offsets().z(), height() - offsets().w(), width() - offsets().z(), 0);// OY
    painter->drawLine(offsets().x(), height() - offsets().w(), width(), height() - offsets().w()); // OX

    if(axis_x.scale_pixel_size <= 0 or axis_y.scale_pixel_size <= 0)
        return;

    float rely_cap = height() - offsets().w() - axis_rely.pixel_offset;
    while(rely_cap > 0)
    {
        if(axis_rely.scale_pixel_size <= 1 or rely_cap <= 1)
            break;
        painter->drawLine(offsets().x() / 2, rely_cap, offsets().x(), rely_cap);
        float alt = ((height() - offsets().w()) - rely_cap) * axis_rely.max * axis_rely.stretch / (height() - offsets().w()) - axis_rely.offset;
        bool meters = alt < 1'000 - 1;
        if(not meters)
            alt /= 1'000;
        painter->drawText(QRectF(0, rely_cap, offsets().x(), 16), QString::number(alt, 'f', 0) + (meters ? "м" : "км"));
        rely_cap -= axis_rely.scale_pixel_size;
    }

    float rely_cap2 = height() - offsets().w() - axis_rely.pixel_offset;
    while(rely_cap2 < height() - offsets().w())
    {
        painter->drawLine(offsets().x() / 2, rely_cap2, offsets().x(), rely_cap2);
        float alt = ((height() - offsets().w()) - rely_cap2) * axis_rely.max * axis_rely.stretch / (height() - offsets().w()) - axis_rely.offset;
        bool meters = alt < 1'000 - 1;
        if(not meters)
            alt /= 1'000;
        painter->drawText(QRectF(0, rely_cap2, offsets().x(), 16), QString::number(alt, 'f', 0) + (meters ? "м" : "км"));
        rely_cap2 += axis_rely.scale_pixel_size;
    }

    float y_cap = height() - offsets().w();
    while(y_cap > 0)
    {
        if(axis_y.scale_pixel_size <= 1 or y_cap <= 1)
            break;
        painter->drawLine(width() - offsets().z(), y_cap, width() - offsets().z() / 2, y_cap);
        float alt = ((height() - offsets().w()) - y_cap) *  axis_y.max *  axis_y.stretch / (height() - offsets().w());
        bool meters = alt < 1'000 - 1;
        if(not meters)
            alt /= 1'000;
        painter->drawText(QRectF(width() - offsets().z() / 1.05, y_cap, offsets().z(), 16), QString::number(alt, 'f', 0) + (meters ? "м" : "км"));
        y_cap -= axis_y.scale_pixel_size;
    }

    float x_cap = offsets().x() - axis_x.max * oxScrollPosition() / (width() * oxScrollSize());
    while(x_cap < width() - offsets().z())
    {
        if(axis_x.scale_pixel_size <= 1 or x_cap <= 1 or width() - offsets().z() <= 1)
            break;
        painter->drawLine(x_cap, height() - offsets().w(), x_cap, height());
        x_cap += axis_x.scale_pixel_size;
        float dist = ((x_cap) *  axis_x.max) / (width() - offsets().z());
        bool meters = dist < 1'000 - 1;
        if(not meters)
            dist /= 1'000;
        int dist_rounded = (int)dist;
        while(dist_rounded % 10)
            dist_rounded--;
        painter->drawText(QRectF(x_cap + 3, height() - offsets().w(), 50, 16), QString::number(dist_rounded) + (meters ? "м" : "км"));
    }
}

void CDeclarativeAxis::set(float stretch, float xmax, float ymax, float yreloffset, float xmin, float ymin)
{
    axis_x.max = xmax; axis_y.max = ymax; axis_rely.max = ymax;
    axis_x.min = xmin; axis_y.min = ymin; axis_rely.min = ymin;
    axis_y.stretch = stretch;

    axis_x.round_max = axis_x.min;
    axis_y.round_max = axis_y.min;
    axis_rely.round_max = axis_rely.min;
    axis_rely.offset = yreloffset;
    axis_rely.pixel_offset = axis_rely.offset * (height() - offsets().w()) / ((float)axis_rely.max * axis_rely.stretch);

    while(axis_x.round_max < axis_x.max)
        axis_x.round_max += pow(10, axis_x.max > 0 ? (int)log10((float) axis_x.max) : 1);
    while(axis_y.round_max < axis_y.max)
        axis_y.round_max += pow(10, axis_y.max > 0 ? (int)log10((float) axis_y.max) : 1);
    while(axis_rely.round_max < axis_rely.max)
        axis_rely.round_max += pow(10, axis_rely.max > 0 ? (int)log10((float)axis_rely.max) : 1);

    axis_x.scale_pixel_size = (width() - offsets().x())
                              / ((float)axis_x.max / (float)pow(10, axis_x.max > 0 ? (int)log10((float) axis_x.max) : 1));
    axis_y.scale_pixel_size = (height() - offsets().w())
                              / ((float)axis_y.max * axis_y.stretch / (float)pow(10, axis_y.max > 0 ? (int)log10((float) axis_y.max) : 1));
    axis_rely.scale_pixel_size = (height() - offsets().w())
                              / ((float)axis_rely.max * axis_rely.stretch / (float)pow(10, axis_rely.max > 0 ? (int)log10((float) axis_rely.max) : 1));
    this->update();
}

QString CDeclarativeAxis::color() const { return m_color; }
void CDeclarativeAxis::setColor(const QString& col) {
    if (m_color == col) return;
    m_color = col;
    emit colorChanged();
    this->update();
}

QVector4D CDeclarativeAxis::offsets() const { return m_offsets; }
void CDeclarativeAxis::setOffsets(const QVector4D& other) {
    if (m_offsets == other) return;
    m_offsets = other;
    emit offsetsChanged();
}

QString CDeclarativeAxis::fontfamily() const { return m_fontfamily; }
void CDeclarativeAxis::setFontfamily(const QString& other) {
    if (m_fontfamily == other) return;
    m_fontfamily = other;
    emit fontfamilyChanged();
}

qreal CDeclarativeAxis::oxScrollPosition() const { return m_oxScrollPosition; }
void CDeclarativeAxis::setOXScrollPosiition(qreal other) {
    if (qFuzzyCompare(m_oxScrollPosition, other)) return;
    m_oxScrollPosition = other;
    emit oxScrollPositionChanged();
    this->update();
}

qreal CDeclarativeAxis::oxScrollSize() const { return m_oxScrollSize; }
void CDeclarativeAxis::setOXScrollSize(qreal other) {
    if (qFuzzyCompare(m_oxScrollSize, other)) return;
    m_oxScrollSize = other;
    emit oxScrollSizeChanged();
    this->update();
}
