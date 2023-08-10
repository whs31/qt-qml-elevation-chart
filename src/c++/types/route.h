#pragma once

#include <vector>
#include <QtPositioning/QGeoPath>
#include "routepoint.h"
#include "elevationpoint.h"

using std::vector;

namespace ElevationChart
{
  /**
   * \brief Набор последовательных точек на карте.
   */
  class Route
  {
    Q_GADGET

    public:
      /// \brief Создает пустой путь.
      Route();

      /**
       * \brief Создает путь из данного QGeoPath с константной скоростью точек.
       * \param path - набор координат с высотой.
       * \param velocity - константная скорость для точек в метрах в секунду.
       * <i>По умолчанию равно нулю</i>.
       */
      explicit Route(const QGeoPath& path, float velocity = 0);

      /**
       * \brief Создает путь из данного вектора точек.
       * \param vec - вектор точек RoutePoint.
       */
      explicit Route(const vector<RoutePoint>& vec);

      /**
       * \brief Добавляет в конец пути точку.
       * \param point - точка для добавления.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void add(const RoutePoint& point);

      /**
       * \brief Добавляет в конец пути точку, созданную из координаты и значения скорости.
       * \param coord - координата точки с высотой.
       * \param velocity - скорость точки в метрах в секунду.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void add(const QGeoCoordinate& coord, float velocity = 0);

      /**
       * \brief Удаляет из пути точку по индексу.
       * \param index - индекс точки для удаления.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void remove(int index);

      /**
       * \brief Удаляет из пути точку по значению
       * \param point - значение точки для удаления.
       * \warning Удалено будет только первое вхождение.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void remove(const RoutePoint& point);

      /**
       * \brief Заменяет точку по индексу.
       * \param index - индекс точки для замены.
       * \param point - новое значение точки.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void replace(int index, const RoutePoint& point);

      /**
       * \brief Очищает путь.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void clear();

      /**
       * \brief Задает путь из QGeoPath с константной скоростью точек.
       * \param path - набор координат с высотой.
       * \param velocity - константная скорость для точек в метрах в секунду.
       * <i>По умолчанию равно нулю</i>.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void set(const QGeoPath& path, float velocity = 0);

      /**
       * \brief Задает путь из вектора точек.
       * \param vec - вектор точек RoutePoint.
       */
      void set(const vector<RoutePoint>& vec);

      /**
       * \brief Задает константную общую скорость каждой точке в пути.
       * \param velocity - скорость в метрах в секунду.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void setVelocity(float velocity);

      /**
       * \brief Задает индивидуальную скорость из массива для каждой точки.
       * \param velocities - массив скоростей точек в метрах в секунду.
       * \param fallback - значение, которое будет использоваться, если массив
       * скоростей меньше, чем количество точек в пути (м/с). <i>По умолчанию равно нулю</i>.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE void setVelocity(const vector<float>& velocities, float fallback = 0);

      /**
       * \brief Возвращает количество точек в пути.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       * \return Количество точек.
       */
      Q_INVOKABLE [[nodiscard]] int size() const;

      /**
       * \brief Преобразует путь в QGeoPath и возвращает его.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       * \return QGeoPath
       */
      Q_INVOKABLE [[nodiscard]] QGeoPath toGeoPath() const;

      /**
       * \brief Преобразует путь в QVariantList и возвращает его.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       * \return QVariantList
       */
      Q_INVOKABLE [[nodiscard]] QVariantList toVariantList() const;

      /**
       * \brief Возвращает точку, лежащую по индексу, или пустую точку, если индекс невалидный.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       * \return Точка пути RoutePoint
       */
      Q_INVOKABLE [[nodiscard]] RoutePoint& at(int index);

      /**
       * \brief Возвращает <tt>true</tt>, если путь содержит указанную точку.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE [[nodiscard]] bool contains(const RoutePoint& point);

      /**
       * \brief Возвращает <tt>true</tt>, если путь корректный.
       * \note Эта функция может быть вызвана из QML через мета-объектную систему Qt.
       */
      Q_INVOKABLE [[nodiscard]] bool valid() const;

      /**
       * \brief Преобразует путь в вектор точек ElevationPoint.
       */
      [[nodiscard]] auto toElevationGraph() const -> vector<ElevationPoint>;

    public:
      /**
       * \brief Создает путь из данного QGeoPath с константной скоростью точек.
       * \param path - набор координат с высотой.
       * \param velocity - константная скорость для точек в метрах в секунду.
       * <i>По умолчанию равно нулю</i>.
       */
      static Route fromGeoPath(const QGeoPath& path, float velocity = 0);

      /**
       * \brief Создает путь из данного вектора точек.
       * \param vec - вектор точек RoutePoint.
       */
      static Route fromPointsVector(const vector<RoutePoint>& vec);

    private:
      vector<RoutePoint> m_vec;
  };
} // ElevationChart

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(ElevationChart::Route)