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
  /**
   * \brief Модель точек пути для профиля высот.
   * \note Класс зарегистрирован как мета-тип и может использоваться в качестве Q_PROPERTY
   * по указателю как в C++, так и в QML.
   */
  class RouteModel : public QAbstractListModel
  {
    Q_OBJECT

    public:
      /// \brief Роли модели.
      enum Roles
      {
        Index = Qt::UserRole + 1,       ///< Индекс точки.
        Altitude,                       ///< Высота точки над уровнем моря в метрах.
        Distance,                       ///< Удаленность точки от начала пути по ломаной прямой в метрах.
        Valid                           ///< Валидность точки.
      };

      /// \brief Создает новый объект модели с указанным родительским QObject.
      explicit RouteModel(QObject* parent = nullptr);

      /// \brief Возвращает количество точек в модели.
      int rowCount(const QModelIndex& parent = QModelIndex()) const override;

      /**
       * \brief Возвращает хранимые в модели данные по индексу и указанной роли.
       * \param index - индекс точки.
       * \param role - типа данных для получения (см. Roles).
       * \return Значение роли по указанному индексу в виде QVariant.
       */
      [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

      /**
       * \brief Задает значение роли для указанной точки по индексу.
       * \param index - индекс точки.
       * \param value - новое значение роли.
       * \param role - роль для изменения (см. Roles).
       * \return Результат выполнения операции.
       */
      bool setData(const QModelIndex& index, const QVariant& value, int role) override;

      /**
       * \brief Добавляет в модель новую точку.
       * \param point - точка для добавления.
       */
      void add(const ElevationPoint& point);

      /**
       * \brief Очищает модель.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void clear();

      /**
       * \brief Изменяет высоту указанной точки.
       * \param index - индекс точки для изменения.
       * \param delta - значения отклонения высоты от текущего значения.
       * При положительных значениях высота точки будет расти, а при отрицательных -
       * падать. Высота не может быть ниже нуля.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
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
      /// \brief Функция для регистрации модели в QML.
      [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    private:
      vector<ElevationPoint> m_storage;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::RouteModel*)