#include "fpscounter.hpp"

#include <QDateTime>
#include <QPainter>
#include <QBrush>

FPSCounter::FPSCounter(QQuickItem *parent): QQuickPaintedItem(parent), _currentFPS(60), _cacheCount(0)
{
    _times.clear();
    setFlag(QQuickItem::ItemHasContents);
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::recalculateFPS()
{
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    _times.push_back(currentTime);

    while (_times[0] < currentTime - 1000) {
        _times.pop_front();
    }

    int currentCount = _times.length();
    _currentFPS = (currentCount + _cacheCount) / 2;

    if (currentCount != _cacheCount) fpsChanged(_currentFPS);

    _cacheCount = currentCount;
}

int FPSCounter::fps()const
{
    return _currentFPS;
}

void FPSCounter::paint(QPainter *painter)
{
    recalculateFPS();
    QBrush brush(Qt::black);

    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(0, 0, boundingRect().width(), boundingRect().height(), 0, 0);
    update();
}
