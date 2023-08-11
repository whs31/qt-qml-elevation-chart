//
// Created by whs31 on 11.08.23.
//

#pragma once
#include <QtCore/QMetaType>

namespace ElevationChart
{
  class Metrics
  {
    Q_GADGET
    Q_PROPERTY(float rateOfClimb READ rateOfClimb WRITE setRateOfClimb FINAL)
    Q_PROPERTY(float rateOfDescend READ rateOfDescend WRITE setRateOfDescend FINAL)
    Q_PROPERTY(float fallbackVelocity READ fallbackVelocity WRITE setFallbackVelocity FINAL)

    public:
      Metrics();
      Metrics(float roc, float rod, float vh);

      [[nodiscard]] float rateOfClimb() const;      void setRateOfClimb(float);
      [[nodiscard]] float rateOfDescend() const;    void setRateOfDescend(float);
      [[nodiscard]] float fallbackVelocity() const; void setFallbackVelocity(float);

    private:
      float m_rateOfClimb;
      float m_rateOfDescend;
      float m_fallbackVelocity;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::Metrics)

namespace ElevationChart
{
  /**
   * \class Metrics
   * \brief Класс-гаджет, хранящий входные данные для расчета коррекции по ЛТХ.
   * \note Класс зарегистрирован как гаджет и может использоваться в качестве Q_PROPERTY
   * как в C++, так и в QML.
   */

  /**
   * \brief Создает объект Metrics со значениями по умолчанию.
   * \details Значения по умолчанию: <tt>[10, 10, 10]</tt>.
   */
  inline Metrics::Metrics()
    : m_rateOfClimb(10.0f)
    , m_rateOfDescend(10.0f)
    , m_fallbackVelocity(10.0f)
  {}

  /**
   * \brief Создает объект Metrics с указанными значениями.
   * \param roc - скороподъемность в м/с.
   * \param rod - скорость спуска в м/с.
   * \param vh - горизонтальная скорость в м/с. Это значение будет использоваться только в том
   * случае, если точка в пути Route не имеет собственного значения скорости.
   */
  inline Metrics::Metrics(float roc, float rod, float vh)
    : m_rateOfClimb(roc)
    , m_rateOfDescend(rod)
    , m_fallbackVelocity(vh)
  {}

  /**
   * \property Bounds::rateOfClimb
   * \brief Скороподъемность БПЛА в м/с.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение                 <th>Запись                  <th>Оповещение
   * <tr><td><i>rateOfClimb</i>     <td><i>setRateOfClimb</i>   <td><i>--</i>
   * </table>
   */
  inline float Metrics::rateOfClimb() const { return m_rateOfClimb; }
  inline void Metrics::setRateOfClimb(float x) { m_rateOfClimb = x; }

  /**
   * \property Bounds::rateOfDescend
   * \brief Скорость спуска БПЛА в м/с.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение                 <th>Запись                  <th>Оповещение
   * <tr><td><i>rateOfDescend</i>   <td><i>setRateOfDescend</i> <td><i>--</i>
   * </table>
   */
  inline float Metrics::rateOfDescend() const { return m_rateOfDescend; }
  inline void Metrics::setRateOfDescend(float x) { m_rateOfDescend = x; }

  /**
   * \property Bounds::fallbackVelocity
   * \brief Горизонтальная скорость в м/с.
   * \details
   * Это значение будет использоваться только в том
   * случае, если точка в пути Route не имеет собственного значения скорости.
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение                 <th>Запись                     <th>Оповещение
   * <tr><td><i>fallbackVelocity</i><td><i>setFallbackVelocity</i> <td><i>--</i>
   * </table>
   */
  inline float Metrics::fallbackVelocity() const { return m_fallbackVelocity; }
  inline void Metrics::setFallbackVelocity(float x) { m_fallbackVelocity = x; }
} // ElevationChart