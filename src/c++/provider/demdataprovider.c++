//
// Created by whs31 on 14.08.23.
//

#include "demdataprovider.h"
#include <algorithm>
#include <DEM/GroundProfile>

namespace ElevationChart
{
  /**
   * \class DEMDataProvider
   * \brief Провайдер данных о профиле высот с использованием библиотеки DEM.
   * \extends IElevationDataProvider
   */

  /**
   * \brief Возвращает массив с точками высотных данных по заданной ломаной прямой.
   * \param path - ломаная прямая, по которой будут рассчитаны высотные данные ландшафта.
   * \return Массив точек ElevationPoint.
   */
  vector<ElevationPoint> DEMDataProvider::plotElevationProfile(const QGeoPath& path) const
  {
    vector<ElevationPoint> ret;
    vector<DEM::GraphPoint> src = DEM::plotTerrainProfile(path);

    ret.reserve(src.size());
    std::transform(src.begin(), src.end(), std::back_inserter(ret), [](const DEM::GraphPoint& point){
      return ElevationPoint(point.distance, point.elevation);
    });

    return ret;
  }
} // ElevationChart