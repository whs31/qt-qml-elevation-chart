#pragma once

#include <QObject>
#include <QGeoPath>

class ElevationWidgetPrivate;
class ElevationWidget : public QObject
{
        Q_OBJECT
    public:
        explicit ElevationWidget(QObject *parent = nullptr);

        /// @brief  Возвращает текущий редактируемый путь из виджета.
        /// @return Заданный путь с неизменными геокоординатами и измененной высотой.
        QGeoPath getGeopath();

        /// @brief Задает путь в виджет для редактирования.
        /// @param path - путь для редактирования.
        void setGeopath(QGeoPath path);

        /// @brief Переключение показа индексов точек на графике.
        /// @param state Состояние показа точек.
        void showIndexes(bool state);

        /// @brief Задает текущую скорость БПЛА в виджет.
        /// @param velocity - скорость в м/с.
        void setVelocity(float velocity);

        /// @brief Задает текущую скороподъемность в виджет.
        /// @param rate - скороподъемность в м/с.
        void setClimbRate(float rate);

        /// @brief Задает текущую скорость спуска в виджет.
        /// @param rate - скорость спуска в м/с. 
        void setDescendRate(float rate);

        /// @brief Задает цветовую палитру виджета.
        /// @param backgroundColor - цвет фона
        /// @param foregroundColor - цвет текста и элементов интерфейса
        /// @param chartColor      - цвет профиля высот
        /// @param successColor    - цвет графика пути и путевых точек
        /// @param warningColor    - цвет графика, скорректированного с учётом ЛТХ БПЛА
        /// @param errorColor      - цвет частей графика, пересекающихся с рельефом местности
        void setPallete(QString backgroundColor, QString foregroundColor, QString chartColor,
                        QString successColor, QString warningColor, QString errorColor);

    protected:
        ElevationWidgetPrivate* const d_ptr;

    private:
        Q_DECLARE_PRIVATE(ElevationWidget)

    signals:
        /// @brief Сигнал, сообщающий об измененении пути.
        void geopathChanged();
};
