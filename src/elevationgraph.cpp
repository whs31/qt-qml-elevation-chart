#include "elevationgraph.hpp"
#include "elevationgraph_p.hpp"

ElevationGraph::ElevationGraph(QObject *parent)
    : QObject{parent}
    , d_ptr(new ElevationGraphPrivate())
{

}

