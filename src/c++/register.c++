//
// Created by whs31 on 03.08.23.
//

#include "register.h"
#include <QtCore/QDebug>
#include <QtQml/qqml.h>
#include "elevationchart.h"
#include "elevationchartapi.h"

namespace ElevationChart
{
  void registerQMLTypes() noexcept
  {
    qmlRegisterModule("ElevationChartWidget", 3, 0);
    qmlRegisterType<ChartItem>("ElevationChartWidget", 3, 0, "ElevationChartImpl");
    qmlRegisterAnonymousType<SG::BasicPalette>("ElevationChartWidget", 3);
    qInfo() << "<elevation-chart> Registered type ElevationChartImpl in module ElevationChartWidget 3.0";
    qInfo() << "<elevation-chart> Registered anonymous type SG::BasicPalette in module ElevationChartWidget 3.X";
  }
} // ElevationChart