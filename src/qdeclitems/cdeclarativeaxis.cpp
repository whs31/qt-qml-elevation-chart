#include "cdeclarativeaxis.hpp"
#include <QPainter>
#include <QBrush>

using namespace ChartsOpenGL;

CDeclarativeAxis::CDeclarativeAxis(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{

}

void CDeclarativeAxis::paint(QPainter* painter)
{
    QColor col(m_color);
    QBrush brush(col);
    QPen pen(brush, 3, Qt::PenStyle::SolidLine, Qt::PenCapStyle::SquareCap, Qt::PenJoinStyle::BevelJoin);

    painter->setBrush(brush);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawLine(offsets().x(), this->height() - offsets().w(), offsets().x(), 0);
    painter->drawLine(offsets().x(), this->height() - offsets().w(), this->width(), this->height() - offsets().w());
}

void CDeclarativeAxis::set(float max, float min)
{
    m_max = max;
    m_min = min;
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
