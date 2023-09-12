#include "randomdataprovider.h"
#include <experimental/random>
#include <QtCore/QDebug>
#include <QtPositioning/QGeoPath>

namespace ElevationChart
{
  /**
   * \class RandomDataProvider
   * \brief Провайдер <b>случайных</b> тестовых данных о профиле высот.
   * \extends IElevationDataProvider
   */

  /**
   * \brief Возвращает массив с точками <b>случайных</b> высотных данных по заданной ломаной прямой.
   * \param path - ломаная прямая, по которой будут рассчитаны высотные данные ландшафта.
   * \return Произвольный массив точек ElevationPoint.
   */
  vector<ElevationPoint> RandomDataProvider::plotElevationProfile(const QGeoPath& path) const
  {
    vector<ElevationPoint> ret;
    int length = static_cast<int>(path.length(0, path.size() - 1));
    qDebug() << "<elevation-chart> Plotting RANDOM chart with length" << length << "meters";

    ret.reserve(length);
    ret.emplace_back();
    for(size_t i = 1; i <= length; ++i)
    {
      auto point = ElevationPoint(i, ret.at(i - 1).elevation() + static_cast<float>(std::experimental::randint(-2, 2)));
      if(point.elevation() < 0)
        point.setElevation(0);
      ret.push_back(point);
    }
    return ret;
  }
} // ElevationChart