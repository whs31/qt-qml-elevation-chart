//
// Created by whs31 on 10.08.23.
//

#pragma once

#include <vector>
#include <QtCore/QAbstractListModel>
#include "types/elevationpoint.h"

using std::vector;

namespace ElevationChart
{
  class RouteModel : public QAbstractListModel
  {
    Q_OBJECT

    public:
      enum Roles                      ///< Роли модели.
      {
        Index = Qt::UserRole + 1,       ///< Индекс точки.
        Altitude,                       ///< Высота точки над уровнем моря в метрах.
        Distance,                       ///< Удаленность точки от начала пути по ломаной прямой в метрах.
        Valid                           ///< Валидность точки.
      };

      explicit RouteModel(QObject* parent = nullptr);

      int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
      bool setData(const QModelIndex& index, const QVariant& value, int role) override;

      void add(const ElevationPoint& point);
      Q_INVOKABLE void clear();
      Q_INVOKABLE void move(int index, float delta);

    signals:
      /**
       * \brief Срабатывает, когда точка была изменена методом move.
       * \details Сигнал необходим, чтобы изменять путь, передаваемый в модель, по
       * сигналу из самой модели.
       * \param index - индекс измененной точки.
       * \param new_altitude - новое значение высоты точки.
       */
      void requireRebuild(int index, float new_altitude);

    protected:
      [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    private:
      vector<ElevationPoint> m_storage;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::RouteModel*)