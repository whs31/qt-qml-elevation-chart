#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include <QVector>
#include <QQuickPaintedItem>

class FPSCounter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
public:
    explicit FPSCounter(QQuickItem *parent = 0);
    ~FPSCounter();
    void paint(QPainter *);
    Q_INVOKABLE int fps()const;

signals:
    void fpsChanged(int);

private:
    void recalculateFPS();
    int _currentFPS;
    int _cacheCount;
    QVector<qint64> _times;
};

#endif // FPSCOUNTER_HPP
