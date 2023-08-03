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
    #ifndef CUSTOM_DEBUG_OUTPUT
    #define CUSTOM_DEBUG_OUTPUT
    qSetMessagePattern("[%{time process}] [%{category}] "
                       "%{if-debug}\033[01;38;05;15m%{endif}"
                       "%{if-info}\033[01;38;05;146m%{endif}"
                       "%{if-warning}\033[1;33m%{endif}"
                       "%{if-critical}\033[1;31m%{endif}"
                       "%{if-fatal}FATAL ERROR \033[1;31m%{endif}%{message}\033[0m");
    #endif

    qmlRegisterModule("ElevationChartWidget", 3, 0);
    qmlRegisterType<ChartItem>("ElevationChartWidget", 3, 0, "ElevationChartImpl");
    qmlRegisterAnonymousType<SG::BasicPalette>("ElevationChartWidget", 3);
    qmlRegisterSingletonInstance<API>("ElevationChartWidget", 3, 0, "ElevationChartCXXAPI", API::get());
    qInfo() << "<elevation-chart> Registered type ElevationChartImpl in module ElevationChartWidget 3.0";
    qInfo() << "<elevation-chart> Registered anonymous type SG::BasicPalette in module ElevationChartWidget 3.X";
    qInfo() << "<elevation-chart> Registered singleton type ElevationChartCXXAPI in module ElevationChartWidget 3.0";
  }
} // ElevationChart