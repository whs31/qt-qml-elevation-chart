//
// Created by whs31 on 03.08.23.
//

#pragma once

#include <memory>
#include <QtCore/QPointF>
#include <QtQuick/QQuickItem>
#include <QtQuick/QSGGeometry>
#include <SG/BasicPalette>
#include "types/route.h"
#include "types/bounds.h"
#include "provider/randomdataprovider.h"
#include "internal/routemodel.h"

using std::unique_ptr;

class QSGGeometryNode;

namespace ElevationChart
{
  /**
   * \property ElevationChartItem::palette
   * \brief Палитра цветов, используемая в виджете.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>palette</i>     <td><i>setPalette</i>   <td><i>paletteChanged</i>
   * </table>
   * \see SG::BasicPalette
   *
   * \property ElevationChartItem::bounds
   * \brief Максимальные значения по осям для виджета.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>bounds</i>      <td><i>setBounds</i>    <td><i>boundsChanged</i>
   * </table>
   * \see ElevationChart::Bounds
   * \warning Запись в свойство из QML приведет к неопределенному поведению.
   *
   * \property ElevationChartItem::route
   * \brief Путь, по которому происходит построения профиля высот.
   * \details
   * Это основное свойство, используемое извне. При обновлении пути виджет перестраивает
   * текущий профиль, а при изменении пути из виджета срабатывает сигнал, и путь можно прочитать
   * обратно.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>route</i>       <td><i>setRoute</i>     <td><i>routeChanged</i>
   * </table>
   * \see ElevationChart::Route
   *
   * \property ElevationChartItem::model
   * \brief Модель точек пути.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>model</i>       <td><i>--</i>           <td><i>--</i>
   * </table>
   * \see ElevationChart::RouteModel
   *
   * \property ElevationChartItem::uavPosition
   * \brief Позиция БПЛА в данный момент.
   * \details
   * Используется для вычисления относительной высоты и отображении позиции БПЛА
   * на виджете.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись               <th>Оповещение
   * <tr><td><i>uavPosition</i> <td><i>setUavPosition</i><td><i>uavPositionChanged</i>
   * </table>
   *
   * \property ElevationChartItem::intersecting
   * \brief Состояние пересечения пути с рельефом.
   * \details
   * Свойство вернет <tt>true</tt>, если путь пересекается с рельефом хотя бы в одной точке.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись                <th>Оповещение
   * <tr><td><i>intersecting</i><td><i>setIntersecting</i><td><i>intersectingChanged</i>
   * </table>
   *
   * \property ElevationChartItem::valid
   * \brief Состояние наличия профилей высот.
   * \details
   * Свойство вернет <tt>false</tt>, если отсутствуют профили высот для выбранного пути.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись                <th>Оповещение
   * <tr><td><i>valid</i>       <td><i>setValid</i>       <td><i>validChanged</i>
   * </table>
   *
   * \property ElevationChartItem::shrinkMode
   * \brief Тип поведения оси Y виджета.
   * \details
   * <i>По умолчанию равняется ShrinkMode::ShrinkToRouteHeight</i>.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение             <th>Запись              <th>Оповещение
   * <tr><td><i>shrinkMode</i>  <td><i>setShrinkMode</i><td><i>shrinkModeChanged</i>
   * </table>
   * \see ShrinkMode
   */

  /**
   * \brief Основной класс библиотеки.
   * \details Представляет собой C++ - реализацию для профиля высот.
   * Класс отвечает за вычисления, отрисовку с аппаратным ускорением и хранение данных о профиле высот.
   * \note Класс зарегистрирован как мета-тип и может использоваться в качестве Q_PROPERTY
   * по указателю как в C++, так и в QML.
   */
  class ElevationChartItem : public QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(SG::BasicPalette palette READ palette WRITE setPalette NOTIFY paletteChanged FINAL)
    Q_PROPERTY(ElevationChart::Bounds bounds READ bounds WRITE setBounds NOTIFY boundsChanged)
    Q_PROPERTY(ElevationChart::Route route READ route WRITE setRoute NOTIFY routeChanged FINAL)
    Q_PROPERTY(ElevationChart::RouteModel* model READ model CONSTANT)
    Q_PROPERTY(QGeoCoordinate uavPosition READ uavPosition WRITE setUavPosition NOTIFY uavPositionChanged FINAL)

    // metrics gadget

    // envelope gadget

    Q_PROPERTY(bool intersecting READ intersecting WRITE setIntersecting NOTIFY intersectingChanged FINAL)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged FINAL)
    // matching metrics
    // is busy

    Q_PROPERTY(int shrinkMode READ shrinkMode WRITE setShrinkMode NOTIFY shrinkModeChanged FINAL)

    constexpr static const float ROUTE_LINE_WIDTH = 5.0f; ///< Ширина линии пути для отрисовки (px).

    public:
      /// \brief Перечисление поведения оси Y.
      enum ShrinkMode
      {
        ShrinkToProfileHeight,    ///< Ось Y будет ограничена сверху высотой профиля высот, либо высотой пути, если таковая больше.
        ShrinkToRouteHeight       ///< Ось Y будет ограничена сверху высотой пути. Если профиль рельефа выше, он будет обрезан.
      };
      Q_ENUM(ShrinkMode)

      /// \brief Создает новый объект с указанным родителем. Этот конструктор должен быть вызван движком QML.
      explicit ElevationChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] SG::BasicPalette palette() const;   void setPalette(SG::BasicPalette);
      [[nodiscard]] Bounds bounds() const;              void setBounds(Bounds);
      [[nodiscard]] Route route() const;                void setRoute(const Route&);
      [[nodiscard]] RouteModel* model() const;
      [[nodiscard]] QGeoCoordinate uavPosition() const; void setUavPosition(const QGeoCoordinate&);

      [[nodiscard]] bool intersecting() const;          void setIntersecting(bool);
      [[nodiscard]] bool valid() const;                 void setValid(bool);

      [[nodiscard]] int shrinkMode() const;             void setShrinkMode(int);

      //Q_INVOKABLE void applyMetricsCorrection() noexcept;
      //Q_INVOKABLE void estimateEnvelope() noexcept;
      //Q_INVOKABLE void applyEnvelopeCorrection() noexcept;

    signals:
      void paletteChanged();
      void boundsChanged();
      void intersectingChanged();
      void validChanged();
      void routeChanged();
      void uavPositionChanged();
      void shrinkModeChanged();

    protected:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override; ///< Цикл отрисовки.
      void requireRecolor();                                                      ///< При вызове этой функции следующий цикл отрисовки обновит палитру цветов.
      void fulfillRecolor();                                                      ///< При вызове этой функции запрос на обновление цветов снимается.

      Q_SLOT void updateProfile() noexcept;                                       ///< Перестраивает профиль рельефа по текущим параметрам.
      Q_SLOT void updateBounds() noexcept;                                        ///< Пересчитывает предельные значения осей. Не перестраивает профиль.

      void handleBackgroundNode() noexcept;                                       ///< Подцикл отрисовки фоновой ноды.
      void handleProfileNode() noexcept;                                          ///< Подцикл отрисовки ноды профиля рельефа.
      void handleRouteNode() noexcept;                                            ///< Подцикл отрисовки ноды пути.

      /**
       * \brief Преобразует координаты ElevationPoint в пиксельные координаты этого объекта.
       * \param x - значение ElevationPoint::distance.
       * \param y - значение ElevationPoint::elevation.
       * \param x_max - максимальное значение по оси X.
       * \param y_max - максимальное значение по оси Y.
       * \return Точка для использования в цикле отрисовки.
       */
      [[nodiscard]] QSGGeometry::Point2D toPixel(float x, float y, float x_max, float y_max) const;

      /**
       * \brief Вспомогательная функция, преобразующая координаты ElevationPoint в пиксельные только по оси X.
       * \param x - значение ElevationPoint::distance.
       * \param x_max - максимальное значение по оси X.
       * \return Значение по оси Х в пикселях.
       */
      [[nodiscard]] float toPixelX(float x, float x_max) const;

      /**
       * \brief Вспомогательная функция, преобразующая координаты ElevationPoint в пиксельные только по оси Y.
       * \param y - значение ElevationPoint::elevation.
       * \param y_max - максимальное значение по оси Y.
       * \return Значение по оси Y в пикселях.
       */
      [[nodiscard]] float toPixelY(float y, float y_max) const;

    protected:
      QSGGeometryNode* m_background_node;       ///< Нода фона.
      QSGGeometryNode* m_profile_node;          ///< Нода профиля рельефа.
      QSGGeometryNode* m_route_node;            ///< Нода пути.

    private:
      bool m_require_recolor;
      SG::BasicPalette m_palette;
      Bounds m_bounds;
      bool m_intersecting;
      bool m_valid;
      Route m_route;
      RouteModel* m_model;
      QGeoCoordinate m_uav_position;

      unique_ptr<RandomDataProvider> m_random_provider;

      vector<ElevationPoint> m_profile;

      ShrinkMode m_shrink_mode;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::ElevationChartItem*);