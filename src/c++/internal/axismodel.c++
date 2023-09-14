//
// Created by whs31 on 14.09.23.
//

#include "axismodel.h"
#include "internal/definitions.h"

namespace ElevationChart
{
  AxisModel::AxisDelegate::AxisDelegate()
    : elevation(0)
    , prefix(PREFIX_METERS)
    , pixel_offset(0)
  {}

  AxisModel::AxisDelegate::AxisDelegate(float elevation, QString prefix, float pixel_offset)
    : elevation(elevation)
    , prefix(std::move(prefix))
    , pixel_offset(pixel_offset)
  {}


  AxisModel::AxisModel(QObject* parent)
    : QAbstractListModel(parent)
  {}

  int AxisModel::rowCount(const QModelIndex& parent) const { return static_cast<int>(m_storage.size()); }
  QVariant AxisModel::data(const QModelIndex& index, int role) const
  {
    if(not index.isValid())
      return {};
    switch(role)
    {
      case Index: return index.row();
      case Elevation: return QVariant::fromValue(m_storage[index.row()].elevation);
      case Prefix: return QVariant::fromValue(m_storage[index.row()].prefix);
      case PixelOffset: return QVariant::fromValue(m_storage[index.row()].pixel_offset);

      default: return MODEL_READ_ERROR;
    }
  }

  QHash<int, QByteArray> AxisModel::roleNames() const
  {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Index] = "index";
    roles[Elevation] = "elevation";
    roles[Prefix] = "prefix";
    roles[PixelOffset] = "pixelOffset";
    return roles;
  }

  void AxisModel::add(float elevation, bool km, float px_offset)
  {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_storage.emplace_back(elevation, km ? PREFIX_KILOMETERS : PREFIX_METERS, px_offset);
    endInsertRows();
  }

  void AxisModel::clear()
  {
    if(m_storage.empty())
      return;
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_storage.clear();
    endRemoveRows();
  }
} // ElevationChart