//
// Created by whs31 on 14.09.23.
//

#include <cmath>
#include "analyzer.h"
#include "elevationchart.h"
#include "internal/definitions.h"

namespace ElevationChart
{

  void Analyzer::analyzeMetrics(ElevationChartItem* source) noexcept
  {
    bool matching_metrics = true;

    QGeoPath correct_path;
    QGeoPath route_path = source->route().toGeoPath();
    if(route_path.isEmpty())
      return;

    auto velocities = source->route().velocities();

    correct_path.addCoordinate(route_path.path().first());
    for(int i = 1; i < route_path.path().size(); i++)
    {
      const bool fallback = velocities.at(i) == 0;
      const auto dx = static_cast<float>(route_path.path().at(i).distanceTo(route_path.path().at(i - 1)));
      const auto dy = static_cast<float>(route_path.path().at(i).altitude() - correct_path.path().at(i - 1).altitude());
      const float dy_min = fallback ? source->metrics().rateOfDescend() * dx / source->metrics().fallbackVelocity()
          : source->metrics().rateOfDescend() * dx / velocities.at(i);
      const float dy_max = fallback ? source->metrics().rateOfClimb() * dx / source->metrics().fallbackVelocity()
          : source->metrics().rateOfClimb() * dx / velocities.at(i);

      auto result = route_path.path().at(i);

      // correct case
      if((dy >= 0 and dy < dy_max) or (dy <= 0 and abs(dy) < dy_min))
      {
        correct_path.addCoordinate(result);
        continue;
      }

        // climbs too fast
      else if(dy > 0 and dy > dy_max)
      {
        result.setAltitude(correct_path.path().at(i - 1).altitude() + dy_max);
        matching_metrics = false;
      }

        // descends too fast
      else if(dy < 0 and abs(dy) > dy_min)
      {
        result.setAltitude(correct_path.path().at(i - 1).altitude() - dy_min);
        matching_metrics = false;
      }

      correct_path.addCoordinate(result);
    }

    if(not matching_metrics)
    {
      source->setMatchingMetrics(false);
      source->m_metrics_path = correct_path;
    }
    else
      source->setMatchingMetrics(true);
  }

  void Analyzer::analyzeUavPosition(ElevationChartItem* source) noexcept
  {
    auto r = source->route().toGeoPath();
    auto path = r.path();
    if(path.empty())
      return;
    float closest_encounter = static_cast<float>(path.front().distanceTo(source->uavPosition()));
    int index = 0;
    for(int i = 1; i < path.size(); i++)
    {
      if(path[i].distanceTo(source->uavPosition()) < closest_encounter)
      {
        closest_encounter = static_cast<float>(path[i].distanceTo(source->uavPosition()));
        index = i;
      }
    }

    source->setUavVisualPosition({ static_cast<float>(source->width() * (r.length(0, index) + closest_encounter) / r.length(0, r.size() - 1)),
                                     source->toPixelY(static_cast<float>(source->uavPosition().altitude()), source->bounds().y()) });
  }

  void Analyzer::analyzeAxes(ElevationChartItem* source) noexcept
  {
    source->yAbsoluteModel()->clear();
    source->yRelativeModel()->clear();

    float exponent = std::floor(std::log10(source->bounds().y()));
    auto spacing = (float)std::pow(10, exponent);
    auto rounded_bound = spacing * 10;
    bool km = rounded_bound > 5'000;
    int j = -1;
    for(float i = 0; i < rounded_bound; i += (spacing / 10))
      source->yAbsoluteModel()->add(km ? i / 1'000 : i, km, source->toPixelY(i, source->bounds().y()), ++j % 10 != 0);

    float delta = static_cast<float>(rounded_bound - source->uavPosition().altitude());
    int k = -1;
    for(float i = source->m_stored_uav_height; i < delta; i += spacing / 10)
      source->yRelativeModel()->add(km ? (i - source->m_stored_uav_height) / 1'000 : i - source->m_stored_uav_height,
                                    km, source->toPixelY(i, source->bounds().y()), ++k % 10 != 0);
  }

  void Analyzer::analyzeBounds(ElevationChartItem* source) noexcept
  {
    if(not source->route().valid())
      return;

    if(source->m_profile.empty())
      return;

    source->updateAxes();

    source->researcher()->researchIntersections(source->route().toGeoPath());

    source->setBounds({ source->m_profile.back().distance(), 0 });
    auto path = source->route().toGeoPath().path();
    for(const auto& coordinate in path)
      if(coordinate.altitude() > source->bounds().y())
        source->setBounds({source->bounds().x(), static_cast<float>(coordinate.altitude())});

    if(source->shrinkMode() == ElevationChartItem::ShrinkToProfileHeight)
      source->setBounds({source->bounds().x(), std::max(source->bounds().y(), std::max_element(source->m_profile.cbegin(), source->m_profile.cend(),
                                                                       [](const ElevationPoint& a, const ElevationPoint& b){ return a.elevation() < b.elevation();
                                                                       })->elevation())});

    source->updateMetrics();
    source->update();
  }
} // ElevationChart