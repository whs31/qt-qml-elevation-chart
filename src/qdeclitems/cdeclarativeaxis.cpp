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
    QColor col(m_color);
    QBrush brush(col);
    QPen pen(brush, 2, Qt::PenStyle::SolidLine, Qt::PenCapStyle::SquareCap, Qt::PenJoinStyle::BevelJoin);

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
        y_cap -= axis_y.scale_pixel_size;
    }

    float x_cap = offsets().x();
    while(x_cap < this->width() - offsets().z())
    {
        painter->drawLine(x_cap, this->height() - offsets().w(), x_cap, this->height());
        x_cap += axis_x.scale_pixel_size;
    }
}

void CDeclarativeAxis::set(float stretch, float xmax, float ymax, float xmin, float ymin)
{
    axis_x.max = xmax; axis_y.max = ymax;
    axis_x.min = xmin; axis_y.min = ymin;

    while(axis_x.round_max < axis_x.max)
        axis_x.round_max += pow(10, axis_x.max > 0 ? (int) log10((float) axis_x.max) : 1);
    while(axis_y.round_max < axis_y.max)
        axis_y.round_max += pow(10, axis_y.max > 0 ? (int) log10((float) axis_y.max) : 1);

    axis_x.scale_pixel_size = (this->width() - offsets().x())
                              / ((float)axis_x.max / (float)pow(10, axis_x.max > 0 ? (int) log10((float) axis_x.max) : 1));
    axis_y.scale_pixel_size = (this->height() - offsets().w())
                              / ((float)axis_y.max * stretch / (float)pow(10, axis_y.max > 0 ? (int) log10((float) axis_y.max) : 1));
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
