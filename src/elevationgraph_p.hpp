#pragma once

#include "elevationgraph.hpp"

class ElevationGraphPrivate
{
    Q_DECLARE_PUBLIC(ElevationGraph)

    public:
        ElevationGraphPrivate() = default;
        virtual ~ElevationGraphPrivate() = default;
        ElevationGraph* q_ptr;

};
