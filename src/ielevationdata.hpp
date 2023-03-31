#pragma once

#include <QObject>

class IElevationData : public QObject
{
    Q_OBJECT
    public:
        static IElevationData* get(QObject* parent = nullptr);

    private:
        explicit IElevationData(QObject* parent);
        static IElevationData* instance;
};
