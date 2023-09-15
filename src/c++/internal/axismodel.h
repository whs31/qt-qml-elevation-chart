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

    constexpr static const char* PREFIX_METERS = "м";
    constexpr static const char* PREFIX_KILOMETERS = "км";

    public:
      enum Roles
      {
        Index = Qt::UserRole + 1,
        Elevation,
        Prefix,
        PixelOffset,
        Minor
      };

      struct AxisDelegate
      {
        AxisDelegate();
        AxisDelegate(float elevation, QString prefix, float pixel_offset, bool minor);

        float elevation;
        QString prefix;
        float pixel_offset;
        bool minor;
      };

      explicit AxisModel(QObject* parent = nullptr);

      [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

      void add(float elevation, bool km, float px_offset, bool minor = false);
      void clear();

    protected:
      [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    private:
      vector<AxisDelegate> m_storage;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::AxisModel*)