//
// Created by whs31 on 03.08.23.
//

#pragma once
#include <QtCore/QObject>
#include "types/route.h"

/**
 * \brief Пространство имен для виджета профиля высот.
 * \todo Гайд по регистрации, qml и прочему (в отдельной статье).
 */
namespace ElevationChart
{
  class ElevationChartItem;

  /**
   * \brief Публичный интерфейс (<b>C++</b>) для виджета профиля высот.
   * \details Класс-синглтон.
   * Предоставляет возможность вызывать функции профиля высот из C++ - кода.
   * Для работы необходимо задать объект, который будет использоваться в C++ - см. setSource.
   * \note Доступен как <i>singleton</i> в QML.
   *    - Модуль: <b>ElevationChartWidget 3.0</b>
   *    - Компонент: <b>ElevationChartCXXAPI 3.0</b>
   */
  class API : public QObject
  {
    Q_OBJECT

    public:
      static API* get();                            ///< Возвращает единственный и уникальный указатель на объект API.

      /**
       * \brief Задает объект, к которому будут направлены обращения из C++ кода в QML.
       * \details Пример:
       * \code {.js}
         ElevationChartImpl
         {
             id: impl
             // ...
         }
         Component.onCompleted: ElevationChartCXXAPI.setSource(impl)
       * \endcode
       *
       * В случае успешного выполнения команды в консоль будет выведено сообщение, например:
       * \code
         <elevation-chart> Source set for C++ wrapper: ElevationChart::ChartItem(0x5607f4e241b0, parent=0x5607f4def790, geometry=0,0 1280x800)
       * \endcode
       *
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       * \warning Если объект не будет задан, поведение виджета не определено. Объект необходимо задать до того,
       * как будет вызвана любая другая функция API (кроме valid).
       * \param item - указатель на объект ChartItem.
       */
      Q_INVOKABLE void setSource(ElevationChart::ElevationChartItem* item);

      /**
       * \brief Возвращает <tt>true</tt>, если С++ обертка имеет валидный указатель на объект ChartItem.
       * \return Состояние оборачиваемого объекта.
       */
      [[nodiscard]] bool valid() const;

      /**
       * \brief Задает путь для построения профиля высот.
       * \param route - набор точек с высотой относительно старта борта и скоростями в м/с.
       */
      void setRoute(const Route& route) noexcept;

      void setUAVPosition(const QGeoCoordinate& coord);

      void setUAVPosition(double latitude, double longitude);



    private:
      explicit API(QObject* parent = nullptr);
      API(const API&);
      API& operator=(const API&);

    private:
      ElevationChartItem* m_source;
  };
} // ElevationChart
