//
// Created by whs31 on 18.08.23.
//

#pragma once

#include <vector>
#include <map>
#include <QtCore/QAbstractListModel>
#include <QtCore/QMetaType>

using std::vector;
using std::map;

namespace ElevationChart
{
  class NotificationModel : public QAbstractListModel
  {
    Q_OBJECT

    public:
      enum Roles                ///< Роли модели.
      {
        Index = Qt::UserRole + 1,     ///< Индекс оповещения.
        Major,                        ///< Является ли оповещение критическим.
        Details,                      ///< Текст оповещения.
        Type                          ///< Тип оповещения (см. NotificationType).
      };

      enum NotificationType
      {
        Default,
        MissingTiles,
        Intersecting,
        MetricsMismatch
      };

      Q_ENUM(NotificationType)

      struct Notification
      {
        int type;
        bool major;
        QString details;
      };

      explicit NotificationModel(QObject* parent = nullptr);

      int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

      Q_INVOKABLE void add(int type, bool is_major);
      Q_INVOKABLE void remove(int type);
      Q_INVOKABLE void clear();

    protected:
      [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    private:
      vector<Notification> m_storage;
      map<NotificationType, QString> m_dictionary;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::NotificationModel*)