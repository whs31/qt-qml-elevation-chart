//
// Created by whs31 on 10.08.23.
//

#include "routemodel.h"

/**
 * \class QAbstractListModel
 * \extends QAbstractItemModel
 *
 * \class QAbstractItemModel
 * \extends QObject
 */

namespace ElevationChart
{
  /**
   * \class RouteModel
   * \brief Модель точек пути для профиля высот.
   * \note Класс зарегистрирован как мета-тип и может использоваться в качестве Q_PROPERTY
   * по указателю как в C++, так и в QML.
   */

  /**
   * \brief Создает новый объект модели с указанным родительским QObject.
   * \param parent - родительский объект в иерархии Qt.
   */
  RouteModel::RouteModel(QObject* parent)
    : QAbstractListModel(parent)
  {}

  /// \brief Возвращает количество точек в модели.
  int RouteModel::rowCount(const QModelIndex& parent) const { return static_cast<int>(m_storage.size()); }

  /**
   * \brief Возвращает хранимые в модели данные по индексу и указанной роли.
   * \param index - индекс точки.
   * \param role - типа данных для получения (см. Roles).
   * \return Значение роли по указанному индексу в виде QVariant.
   */
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

      default: return "Error reading from model";
    }
  }

  /**
   * \brief Задает значение роли для указанной точки по индексу.
   * \param index - индекс точки.
   * \param value - новое значение роли.
   * \param role - роль для изменения (см. Roles).
   * \return Результат выполнения операции.
   */
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

      default: return false;

      emit dataChanged(index, index, {role});
      return true;
    }

    return false;
  }

  /**
   * \brief Добавляет в модель новую точку.
   * \param point - точка для добавления.
   */
  void RouteModel::add(const ElevationPoint& point)
  {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_storage.push_back(point);
    endInsertRows();
  }

  /**
   * \brief Очищает модель.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void RouteModel::clear()
  {
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_storage.clear();
    endRemoveRows();
  }

  /// \brief Функция для регистрации модели в QML.
  QHash<int, QByteArray> RouteModel::roleNames() const
  {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Index] = "index";
    roles[Altitude] = "altitude";
    roles[Distance] = "distance";
    roles[Valid] = "valid";
    return roles;
  }

  /**
   * \brief Изменяет высоту указанной точки.
   * \param index - индекс точки для изменения.
   * \param delta - значения отклонения высоты от текущего значения.
   * При положительных значениях высота точки будет расти, а при отрицательных -
   * падать. Высота не может быть ниже нуля.
   * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
   */
  void RouteModel::move(int i, float delta)
  {
    if(i < 0 or i >= rowCount())
      return;
    m_storage.at(i).setElevation(m_storage.at(i).elevation() + delta / 30);
    if(m_storage.at(i).elevation() <= 0)
      m_storage.at(i).setElevation(0);
    emit dataChanged(index(i), index(i), {Altitude});
    emit requireRebuild(i, m_storage.at(i).elevation());
  }
} // ElevationChart