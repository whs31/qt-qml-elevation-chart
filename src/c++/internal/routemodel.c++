//
// Created by whs31 on 10.08.23.
//

#include "routemodel.h"

namespace ElevationChart
{
  RouteModel::RouteModel(QObject* parent)
    : QAbstractListModel(parent)
  {}

  int RouteModel::rowCount(const QModelIndex& parent) const { return static_cast<int>(m_storage.size()); }
  QVariant RouteModel::data(const QModelIndex& index, int role) const
  {
    if(not index.isValid())
      return {};
    switch(role)
    {
      case Index: return index.row();
      case Altitude: return QVariant::fromValue(m_storage.at(index.row()).elevation());
      case Distance: return QVariant::fromValue(m_storage.at(index.row()).distance());
      case Valid: return QVariant::fromValue(m_storage.at(index.row()).valid());
      case Intersects: return QVariant::fromValue(m_storage.at(index.row()).intersects());

      default: return "Error reading from model";
    }
  }

  bool RouteModel::setData(const QModelIndex& index, const QVariant& value, int role)
  {
    if(data(index, role) == value)
      return false;

    switch(role)
    {
      case Index: return false;
      case Altitude: m_storage.at(index.row()).setElevation(value.toFloat()); break;
      case Distance: m_storage.at(index.row()).setDistance(value.toFloat()); break;
      case Valid: m_storage.at(index.row()).setValid(value.toBool()); break;
      case Intersects: m_storage.at(index.row()).setIntersects(value.toBool()); break;

      default: return false;

      emit dataChanged(index, index, {role});
      return true;
    }

    return false;
  }

  void RouteModel::add(const ElevationPoint& point)
  {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_storage.push_back(point);
    endInsertRows();
  }

  void RouteModel::clear()
  {
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_storage.clear();
    endRemoveRows();
  }

  QHash<int, QByteArray> RouteModel::roleNames() const
  {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Index] = "index";
    roles[Altitude] = "altitude";
    roles[Distance] = "distance";
    roles[Valid] = "valid";
    roles[Intersects] = "intersects";
    return roles;
  }

  void RouteModel::move(int i, float delta)
  {
    if(i < 0 or i >= rowCount())
      return;
    m_storage.at(i).setElevation(m_storage.at(i).elevation() + delta / 100);
    if(m_storage.at(i).elevation() <= 0)
      m_storage.at(i).setElevation(0);
    emit dataChanged(index(i), index(i), {Altitude});
  }
} // ElevationChart