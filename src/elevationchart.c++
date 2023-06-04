#include "elevationchart.h"

namespace Widgets
{

ElevationChart::ElevationChart(QQuickItem* parent)
    : LPVL::ChartBase{parent}
{
    setFlag(ItemHasContents);
}

} // Widgets
