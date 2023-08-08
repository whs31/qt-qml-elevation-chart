//
// Created by whs31 on 03.08.23.
//

#pragma once

#include <QtCore/QObject>
#include "types/route.h"

namespace ElevationChart
{
  void registerQMLTypes() noexcept;

  class TypeFactory : public QObject
  {
    Q_OBJECT

    public:
      explicit TypeFactory(QObject* parent = nullptr);

      Q_INVOKABLE static ElevationChart::RoutePoint routePoint(double latitude, double longitude, float elevation = 0, float velocity = 0);
      Q_INVOKABLE static ElevationChart::Route route(const QGeoPath& path, float velocity = 0);
  };
} // ElevationChart
