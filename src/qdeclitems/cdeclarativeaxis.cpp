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

    painter->drawLine(offsets().x(), this->height() - offsets().w(), offsets().x(), 0);                              // OY
    painter->drawLine(offsets().x(), this->height() - offsets().w(), this->width(), this->height() - offsets().w()); // OX


    if(axis_x.scale_pixel_size <= 0 or axis_y.scale_pixel_size <= 0)
        return;

    float y_cap = this->height() - offsets().w();
    while(y_cap > 0)
    {
        painter->drawLine(offsets().x(), y_cap, offsets().x() / 2, y_cap);
        float alt = ((this->height() - offsets().w()) - y_cap) *  axis_y.max *  axis_y.stretch / (this->height() - offsets().w());
        bool meters = alt < 1'000 - 1;
        if(not meters)
            alt /= 1'000;
        painter->drawText(QRectF(0, y_cap, offsets().x(), 16), QString::number(alt, 'f', 0) + (meters ? "м" : "км"));
        y_cap -= axis_y.scale_pixel_size;
    }

    float x_cap = offsets().x();
    while(x_cap < this->width() - offsets().z())
    {
        painter->drawLine(x_cap, this->height() - offsets().w(), x_cap, this->height());
        x_cap += axis_x.scale_pixel_size;
        float dist = (x_cap) *  axis_x.max / (this->width() - offsets().z());
        bool meters = dist < 1'000 - 1;
        if(not meters)
            dist /= 1'000;
        int dist_rounded = (int)dist;
        dist_rounded += (dist_rounded % 10 == 9 ? -1 : (dist_rounded % 10 == 1 ? 1 : 0));
        painter->drawText(QRectF(x_cap + 3, this->height() - offsets().w(), 50, 16), QString::number(dist_rounded) + (meters ? "м" : "км")); //a
    }
}

void CDeclarativeAxis::set(float stretch, float xmax, float ymax, float xmin, float ymin)
{
    axis_x.max = xmax; axis_y.max = ymax;
    axis_x.min = xmin; axis_y.min = ymin;
    axis_y.stretch = stretch;

    while(axis_x.round_max < axis_x.max)
        axis_x.round_max += pow(10, axis_x.max > 0 ? (int) log10((float) axis_x.max) : 1);
    while(axis_y.round_max < axis_y.max)
        axis_y.round_max += pow(10, axis_y.max > 0 ? (int) log10((float) axis_y.max) : 1);

    axis_x.scale_pixel_size = (this->width() - offsets().x())
                              / ((float)axis_x.max / (float)pow(10, axis_x.max > 0 ? (int) log10((float) axis_x.max) : 1));
    axis_y.scale_pixel_size = (this->height() - offsets().w())
                              / ((float)axis_y.max * axis_y.stretch / (float)pow(10, axis_y.max > 0 ? (int) log10((float) axis_y.max) : 1));
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
