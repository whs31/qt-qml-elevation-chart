#pragma once

#include "ielevationdataprovider.h"

namespace ElevationChart
{
  /// \brief Провайдер <b>случайных</b> тестовых данных о профиле высот.
  class RandomDataProvider : public IElevationDataProvider
  {
    public:
      /**
       * \brief Возвращает массив с точками <b>случайных</b> высотных данных по заданной ломаной прямой.
       * \param path - ломаная прямая, по которой будут рассчитаны высотные данные ландшафта.
       * \return Произвольный массив точек ElevationPoint.
       */
      [[nodiscard]] vector<ElevationPoint> plotElevationProfile(const QGeoPath& path) const override;
  };
} // ElevationChart