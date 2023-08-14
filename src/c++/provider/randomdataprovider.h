#pragma once

#include "ielevationdataprovider.h"

namespace ElevationChart
{
  class RandomDataProvider : public IElevationDataProvider
  {
    public:
      [[nodiscard]] vector<ElevationPoint> plotElevationProfile(const QGeoPath& path) const override;
  };
} // ElevationChart