//
// Created by whs31 on 14.09.23.
//

#pragma once

namespace ElevationChart
{
  class ElevationChartItem;
  class Analyzer
  {
    public:
      static void analyzeMetrics(ElevationChartItem* source) noexcept;
      static void analyzeUavPosition(ElevationChartItem* source) noexcept;
      static void analyzeAxes(ElevationChartItem* source) noexcept;
      static void analyzeBounds(ElevationChartItem* source) noexcept;
  };
} // ElevationChart
