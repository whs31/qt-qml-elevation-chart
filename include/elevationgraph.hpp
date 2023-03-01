#pragma once

#include <QObject>

class ElevationGraphPrivate;
class ElevationGraph : public QObject
{
    Q_OBJECT
    public:
        explicit ElevationGraph(QObject *parent = nullptr);

    protected:
        ElevationGraphPrivate* const d_ptr;

    private:
        Q_DECLARE_PRIVATE(ElevationGraph)

    signals:

};
