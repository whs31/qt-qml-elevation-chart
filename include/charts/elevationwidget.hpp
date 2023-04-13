#pragma once

#include "geopoint.hpp"

#include <QObject>
#include <list>
#define __signal void

using std::list;
class QQuickItem;

/// @namespace Пространство имен для библиотеки построения графиков
namespace GLCharts
{
    /// @private Приватная имплементация профиля высот.
    class ElevationWidgetPrivate;
    /// @brief API для виджета профиля высот.
    class ElevationWidget : public QObject
    {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ElevationWidget)

        public:
            explicit ElevationWidget(QObject *parent);

            //! @brief Связывает QML и C++ части виджета в контексте целевого приложения.
            //! @param rootObject - указатель на  Item в QML, в котором размещен виджет.
            //! @details Конструктор этого класса должен быть вызван до инициализации UI
            //!          (для регистрации типов QML). Эта функция должна быть вызвана после
            //!          инициализации QML и UI.
            //! @warning Без вызова этой функции работать ничего не будет.
            void linkWithQML(QQuickItem* rootObject);

            //! @defgroup Маршрут
            //! @{
            //! @brief Возвращает текущий редактируемый путь из виджета.
            //! @return список геоточек с высотой относительно борта и скоростями.
            list<GeoPoint> getRoute();

            //! @brief Задает список точек в виджет.
            //! @param route - список точек с высотой отн. борта и скоростями в м/с.

            void setRoute(const list<GeoPoint>& route);

            //! @brief Задает географические координаты БПЛА для расчета высоты для точек.
            //! @param uav_position - геокоординаты БПЛА. Высота в них не будет учитываться.
            void setUAVPosition(const QGeoCoordinate& position);

            //! @overload
            //! @param uav_latitude - географическая широта БПЛА.
            //! @param uav_longitude - географическая долгота БПЛА.
            void setUAVPosition(double latitude, double longitude);

            //! @brief Возвращает TRUE, если заданный список точек пересекает рельеф хотя бы в одной точке.
            bool isIntersecting();

            //! @brief Возвращает FALSE, если в папке с профилями высот отсутствуют нужные файлы.
            bool isValid();
            //! @}



            //! @defgroup Коррекция по ЛТХ
            //! @{
            //! @brief Задает максимально допустимую скороподъемность.
            //! @param rate - скорость в м/с.
            void setClimbRate(float rate);

            //! @brief Задает максимально допустимую скорость спуска.
            //! @param rate - скорость в м/с.
            void setDescendRate(float rate);

            //! @brief Задает константную скорость для вычислений ЛТХ/огибающей (см. подробности).
            //! @param velocity - скорость в м/с.
            //! @details Скорость будет использована при:
            //!              * вычислении огибающей: точкам результата вычисления огибающей будет задана эта скорость
            //!                в качестве скорости по умолчанию;
            //!              * в случае, если скорость в точке пути невалидна (например, задано отрицательное значение).
            //!                Тогда этой точке будет присвоена скорость по умолчанию.
            void setGlobalVelocity(float velocity);

            //! @brief Применяет к текущему редактируемому пути коррекцию по ЛТХ.
            void applyMetricsCorrection();

            //! @brief Возвращает TRUE, если редактируемый список точек удовлетворяет требованиям по ЛТХ.
            bool isMatchingMetrics();
            //! @}



            //! @defgroup Огибающая рельефа
            //! @{
            //! @brief Задает предпочитаемую высоту для коридора огибания.
            //! @param altitude - высота в метрах.
            void setEnvelopeMinimumAltitude(float altitude);

            //! @brief Задает предпочитаемый размер коридора огибания.
            //! @param distance - расстояние между нижней и верхней границей профиля огибания в метрах.
            void setEnvelopeCoridorSize(float distance);

            //! @brief Вычисляет маршрут огибания и отображает его на экране.
            void estimateEnvelope();

            //! @brief Применяет вычисленный маршрут огибания к текущему загруженному маршруту.
            void applyEnvelopeCorrection();
            //! @}

            signals:
                //! @brief Сигнал, сообщающий об изменении набора точек в виджете.
                __signal routeChanged();

                //! @brief Сигнал, сообщающий, что состояние пересечения изменилось.
                //! @param intersects - состояние пересечения.
                __signal intersectingStateChanged(bool intersects);

                //! @brief Сигнал посылается при завершении вычислений в виджете.
                __signal processFinished();

                //! @brief Сигнал посылается при изменении состояния расчета огибающей.
                //! @param state - состояние расчета огибающей.
                __signal envelopeCalculated(bool state);

        protected:
            ElevationWidgetPrivate* const d_ptr;
    };
} // namespace charts;
