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
    QBrush brush(QColor("#121212"));

    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);

    QSizeF itemSize = this->size();
    painter->drawRoundedRect(0, 0, itemSize.width() / 3, itemSize.height() - 100, 10, 10);
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
}
