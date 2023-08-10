//
// Created by whs31 on 10.08.23.
//

#pragma once

#include <vector>
#include <QtCore/QAbstractListModel>
#include "types/elevationpoint.h"

using std::vector;

/**
 * \class QAbstractListModel
 * \extends QAbstractItemModel
 *
 * \class QAbstractItemModel
 * \extends QObject
 */

namespace ElevationChart
{
  class RouteModel : public QAbstractListModel
  {
    Q_OBJECT

    public:
      enum Roles
      {
        Index = Qt::UserRole + 1,
        Altitude,
        Distance,
        Valid,
        Intersects
      };

      explicit RouteModel(QObject* parent = nullptr);

      int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
      bool setData(const QModelIndex& index, const QVariant& value, int role) override;

      void add(const ElevationPoint& point);
      void clear();

    protected:
      [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    private:
      vector<ElevationPoint> m_storage;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::RouteModel*)