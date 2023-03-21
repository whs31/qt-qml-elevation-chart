#pragma once

#include <QObject>
#include <QGeoPath>
#include <vector>

/// @private Приватная имплементация профиля высот.
class ElevationWidgetPrivate;
/// @brief API для виджета профиля высот.
class ElevationWidget : public QObject
{
        Q_OBJECT
    public:
        explicit ElevationWidget(QObject *parent = nullptr);

        /// @brief  Возвращает текущий редактируемый путь из виджета.
        /// @return - заданный путь с неизменными геокоординатами и измененной высотой.
        QGeoPath getGeopath();

        /// @brief Задает путь в виджет для редактирования.
        /// @param path - путь для редактирования.
        void setGeopath(const QGeoPath& path);

        /// @brief Возвращает true, если обычный путь и скорректированный путь с учётом ЛТХ совпадают.
        bool isPathMatchingMetrics(void);

        /// @brief  Меняет точки основного пути на точки, скорректированные с учётом ЛТХ и возвращает путь.
        /// @return - скорректированный путь.
        QGeoPath applyMetricsCorrection(void);

        /// @brief  Вычисляет маршрут огибания и применяет его к текущему загруженному маршруту.
        /// @return - вычисленный маршрут
        void applyTerrainEnvelope(void);

        /// @brief Переключение показа индексов точек на графике.
        /// @param state - состояние показа точек.
        void showIndexes(bool state);

        /// @brief Задает текущую скорость БПЛА в виджет.
        /// @param velocity - скорость в м/с.
        void setVelocity(float velocity);
        /// @overload Задает индивидуальные скорости для каждой точки на пути.
        void setVelocity(const std::vector<uint8_t>& points);

        /// @brief Задает текущую скороподъемность в виджет.
        /// @param rate - скороподъемность в м/с.
        void setClimbRate(float rate);

        /// @brief Задает текущую скорость спуска в виджет.
        /// @param rate - скорость спуска в м/с. 
        void setDescendRate(float rate);

        /// @brief Задает высоту для коридора огибания.
        /// @param height - высота в метрах.
        void setBoundHeight(float height);

        /// @brief Задает ширину коридора огибания.
        /// @param width - расстояние между нижней и верхней границей профиля огибания в метрах.
        void setBoundWidth(float width);

        /// @brief Возвращает true, если путь пересекается с профилем высот.
        bool isIntersecting(void);

        /// @brief Вовращает false, если в папке с профилями высот отсутствуют нужные файлы.
        bool isValidPath(void);

        /// @brief Задает цветовую палитру виджета.
        /// @param backgroundColor - цвет фона;
        /// @param foregroundColor - цвет текста и элементов интерфейса;
        /// @param chartColor      - цвет профиля высот;
        /// @param successColor    - цвет графика пути и путевых точек;
        /// @param warningColor    - цвет графика, скорректированного с учётом ЛТХ БПЛА;
        /// @param errorColor      - цвет частей графика, пересекающихся с рельефом местности;
        void setPallete(QString backgroundColor, QString foregroundColor, QString chartColor,
                        QString successColor, QString warningColor, QString errorColor);

    protected:
        ElevationWidgetPrivate* const d_ptr;

    private:
        Q_DECLARE_PRIVATE(ElevationWidget)

    signals:
        /// @brief Сигнал, сообщающий об измененении пути.
        void geopathChanged();

        /// @brief Сигнал сообщает, что состояние пересечения пути с рельефом изменилось.
        /// @details Нужно запрашивать isIntersecting() при получении сигнала.
        void intersectingStateChanged();
};
