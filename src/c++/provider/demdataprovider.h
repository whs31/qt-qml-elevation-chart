//
// Created by whs31 on 14.08.23.
//

#pragma once

#include "ielevationdataprovider.h"

namespace ElevationChart
{
  class DEMDataProvider : public IElevationDataProvider
  {
    public:
      [[nodiscard]] vector<ElevationPoint> plotElevationProfile(const QGeoPath& path) const override;
  };
} // ElevationChart