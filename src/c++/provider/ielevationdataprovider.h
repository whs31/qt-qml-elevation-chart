#pragma once

#include <vector>
#include "types/elevationpoint.h"

class QGeoPath;

using std::vector;

namespace ElevationChart
{
  /**
   * \brief Интерфейс для провайдера данных о профиле высот ландшафта.
   */
  class IElevationDataProvider
  {
    public:
      IElevationDataProvider() = default;
      virtual ~IElevationDataProvider() = default;

      /**
       * \brief Возвращает массив с точками высотных данных по заданной ломаной прямой.
       * \param path - ломаная прямая, по которой будут рассчитаны высотные данные ландшафта.
       * \return Массив точек ElevationPoint.
       */
      [[nodiscard]] virtual vector<ElevationPoint> plotElevationProfile(const QGeoPath& path) const = 0;
  };
} // ElevationChart