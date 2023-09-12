//
// Created by whs31 on 18.08.23.
//

#include "notificationmodel.h"

namespace ElevationChart
{
  /**
   * \class NotificationModel
   * \brief Модель оповещений об ошибках в маршруте.
   * \note Класс зарегистрирован как мета-тип и может использоваться в качестве Q_PROPERTY
   * по указателю как в C++, так и в QML.
   */

  /**
   * \brief Создает новый объект модели с указанным родительским QObject.
   * \param parent - родительский объект в иерархии Qt.
   */
  NotificationModel::NotificationModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_dictionary({
      {Default, "Ошибка!"},
      {MissingTiles, "Отсутствуют карты профиля высот!"},
      {Intersecting, "Маршрут пересекает рельеф!"},
      {MetricsMismatch, "Маршрут не соответствует ЛТХ борта"}
    })
  {}

  /// \brief Возвращает количество точек в модели.
  int NotificationModel::rowCount(const QModelIndex& parent) const { return static_cast<int>(m_storage.size()); }

  /**
   * \brief Возвращает хранимые в модели данные по индексу и указанной роли.
   * \param index - индекс оповещения.
   * \param role - типа данных для получения (см. Roles).
   * \return Значение роли по указанному индексу в виде QVariant.
   */
  QVariant NotificationModel::data(const QModelIndex& index, int role) const
  {
    if(not index.isValid())
      return {};
    switch(role)
    {
      case Index: return index.row();
      case Major: return QVariant::fromValue(m_storage.at(index.row()).major);
      case Details: return QVariant::fromValue(m_storage.at(index.row()).details);

      default: return "Error reading from model";
    }
  }

  void NotificationModel::add(int type, bool is_major)
  {
    if(std::any_of(m_storage.cbegin(), m_storage.cend(), [type](const Notification& x){ return type == x.type; }))
      return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_storage.push_back({type, is_major, m_dictionary[static_cast<NotificationType>(type)]});
    endInsertRows();
  }

  void NotificationModel::remove(int type)
  {
    int index = -1;
    int i = 0;
    for(const auto& notification : m_storage)
    {
      if(notification.type == type)
      {
        index = i;
        break;
      }
      i++;
    }

    if(index < 0 or index >= rowCount())
      return;

    beginRemoveRows(QModelIndex(), index, index);
    m_storage.erase(m_storage.begin() + index);
    endRemoveRows();
  }

  /**
   * \brief Очищает модель.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void NotificationModel::clear()
  {
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_storage.clear();
    endRemoveRows();
  }

  /// \brief Функция для регистрации модели в QML.
  QHash<int, QByteArray> NotificationModel::roleNames() const
  {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Index] = "index";
    roles[Major] = "major";
    roles[Details] = "details";
    return roles;
  }
} // ElevationChart