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
    #if !defined(Q_OS_WIN) && !defined(CUSTOM_DEBUG_OUTPUT)
    #define CUSTOM_DEBUG_OUTPUT
    qSetMessagePattern("[%{time process}] [%{category}] "
                       "%{if-debug}\033[01;38;05;15m%{endif}"
                       "%{if-info}\033[01;38;05;146m%{endif}"
                       "%{if-warning}\033[1;33m%{endif}"
                       "%{if-critical}\033[1;31m%{endif}"
                       "%{if-fatal}FATAL ERROR \033[1;31m%{endif}%{message}\033[0m");
    #endif

    qmlRegisterModule("ElevationChartWidget", 3, 0);
    qmlRegisterType<ElevationChartItem>("ElevationChartWidget", 3, 0, "ElevationChartImpl");
    qmlRegisterAnonymousType<LPVL::BasicPalette>("ElevationChartWidget", 3);
    qRegisterMetaType<Route>("ElevationChart::Route");
    qRegisterMetaType<RoutePoint>("ElevationChart::RoutePoint");
    qmlRegisterSingletonInstance<API>("ElevationChartWidget", 3, 0, "ElevationChartCXXAPI", API::get());
    qmlRegisterSingletonInstance<TypeFactory>("ElevationChartWidget", 3, 0, "Types", new TypeFactory(API::get()));

    qInfo() << "<elevation-chart> Registered type ElevationChartImpl in module ElevationChartWidget 3.0";
    qInfo() << "<elevation-chart> Registered anonymous type LPVL::BasicPalette in module ElevationChartWidget 3.X";
    qInfo() << "<elevation-chart> Registered singleton type ElevationChartCXXAPI in module ElevationChartWidget 3.0";
    qInfo() << "<elevation-chart> Registered singleton type Types in module ElevationChartWidget 3.0";
  }

  TypeFactory::TypeFactory(QObject* parent)
    : QObject(parent)
  {}

  RoutePoint TypeFactory::routePoint(double la, double lo, float e, float v) { return {la, lo, e, v}; }
  Route TypeFactory::route(const QGeoPath& path, float velocity) { return Route(path, velocity); }
  Route TypeFactory::route(const QGeoPath& path, const QVector<float>& velocities)
  {
    auto ret = Route(path);
    ret.setVelocity(vector(velocities.begin(), velocities.end()), 0);
    return ret;
  }
} // ElevationChart