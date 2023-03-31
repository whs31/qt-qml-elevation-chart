#include "ielevationdata.hpp"
#include <qqml.h>

IElevationData* IElevationData::instance = nullptr;
IElevationData* IElevationData::get(QObject* parent) {
    if(instance)
        return instance;
    instance = new IElevationData(parent);
    return instance;
}

IElevationData::IElevationData(QObject *parent)
    : QObject{parent}
{

}
