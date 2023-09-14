//
// Created by whs31 on 14.09.23.
//

#pragma once

#include <vector>
#include <QtCore/QAbstractListModel>
#include <QtCore/QMetaType>

using std::vector;

namespace ElevationChart
{
  class AxisModel : public QAbstractListModel
  {
    Q_OBJECT

    public:
      enum Roles
      {
        Index = Qt::UserRole + 1,
        Elevation,
        Prefix,
        PixelOffset
      };
  };
} // ElevationChart
