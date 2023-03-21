#pragma once

#include <QObject>
#include <QGeoPath>
#include <vector>

/// @namespace Пространство имен для библиотеки построения графиков
namespace Charts
{
    /// @private Приватная имплементация профиля высот.
    class ElevationWidgetPrivate;
    /// @brief API для виджета профиля высот.
    class ElevationWidget : public QObject
    {
        Q_OBJECT
        public:
            explicit ElevationWidget(QObject *parent = nullptr);

            /// @defgroup Маршрут
            /// @{
            /// @brief  Возвращает текущий редактируемый путь из виджета.
            /// @return - заданный путь с неизменными геокоординатами и измененной высотой.
            QGeoPath getGeopath();

            /// @brief Задает путь в виджет для редактирования.
            /// @param path - путь для редактирования.
            void setGeopath(const QGeoPath& path);

            /// @brief Возвращает true, если обычный путь и скорректированный путь с учётом ЛТХ совпадают.
            bool isPathMatchingMetrics(void);

            /// @brief Возвращает true, если путь пересекается с профилем высот.
            bool isIntersecting(void);

            /// @brief Вовращает false, если в папке с профилями высот отсутствуют нужные файлы.
            bool isValidPath(void);
            /// @}



            /// @defgroup Коррекция по ЛТХ
            /// @{
            /// @brief Задает текущую скорость БПЛА в виджет.
            /// @param velocity - скорость в м/с.
            void setVelocity(float velocity);
            /// @overload Задает индивидуальные скорости для каждой точки на пути.
            void setVelocity(const std::vector<uint8_t>& points);

            /// @brief  Возвращает значение скорости для каждой точки на графике
            /// @return - вектор, хранящий скорости для точек. Значение скорости будет константно, если график перестроен с учетом огибания
            std::vector<uint8_t> getVelocity();

            /// @brief Задает текущую скороподъемность в виджет.
            /// @param rate - скороподъемность в м/с.
            void setClimbRate(float rate);

            /// @brief Задает текущую скорость спуска в виджет.
            /// @param rate - скорость спуска в м/с.
            void setDescendRate(float rate);

            /// @brief  Меняет точки основного пути на точки, скорректированные с учётом ЛТХ и возвращает путь.
            /// @return - скорректированный путь.
            QGeoPath applyMetricsCorrection(void);
            /// @}



            /// @defgroup Огибание рельефа
            /// @{
            /// @brief Задает предпочитаемую высоту для коридора огибания.
            /// @param height - высота в метрах.
            void setEnvelopeMinHeight(float height);

            /// @brief Задает размер коридора огибания.
            /// @param height - расстояние между нижней и верхней границей профиля огибания в метрах.
            void setEnvelopeCoridorSize(float distance);

            /// @brief Вычисляет маршрут огибания и отображает его на экране
            void calculateTerrainEnvelope(void);

            /// @brief Применяет вычисленный маршрут огибания к текущему загруженному маршруту.
            void applyTerrainEnvelope(void);
            /// @}



            /// @defgroup Графический интерфейс
            /// @{
            /// @brief Переключение показа индексов точек на графике.
            /// @param state - состояние показа точек.
            void showIndexes(bool state);

            /// @brief Задает цветовую палитру виджета.
            /// @param backgroundColor - цвет фона;
            /// @param foregroundColor - цвет текста и элементов интерфейса;
            /// @param chartColor      - цвет профиля высот;
            /// @param successColor    - цвет графика пути и путевых точек;
            /// @param warningColor    - цвет графика, скорректированного с учётом ЛТХ БПЛА;
            /// @param errorColor      - цвет частей графика, пересекающихся с рельефом местности;
            /// @param subColor        - дополнительный цвет для прочих частей графика (например, для профиля огибания).
            void setPallete(QString backgroundColor, QString foregroundColor, QString chartColor,
                            QString successColor, QString warningColor, QString errorColor, QString subColor);
            /// @}

            signals:
            /// @defgroup Сигналы
            /// @{
            /// @brief Сигнал, сообщающий об измененении пути.
            void geopathChanged();

            /// @brief Сигнал сообщает, что состояние пересечения пути с рельефом изменилось.
            /// @details Нужно запрашивать isIntersecting() при получении сигнала.
            void intersectingStateChanged();
            /// @}

        protected:
            ElevationWidgetPrivate* const d_ptr;

        private:
            Q_DECLARE_PRIVATE(ElevationWidget)
    };
} //namespace charts
