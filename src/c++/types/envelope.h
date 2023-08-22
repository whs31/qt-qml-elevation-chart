//
// Created by whs31 on 11.08.23.
//

#pragma once
#include <QtCore/QMetaType>

namespace ElevationChart
{
  class Envelope
  {
    Q_GADGET
    Q_PROPERTY(float altitude READ altitude WRITE setAltitude FINAL)
    Q_PROPERTY(float width READ width WRITE setWidth FINAL)

    public:
      Envelope();
      Envelope(float altitude, float width);

      [[nodiscard]] float altitude() const;      void setAltitude(float);
      [[nodiscard]] float width() const;         void setWidth(float);

    private:
      float m_altitude;
      float m_width;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::Envelope)

namespace ElevationChart
{
  /**
   * \class Envelope
   * \brief Класс-гаджет, хранящий входные данные для расчета огибающей рельефа.
   * \note Класс зарегистрирован как гаджет и может использоваться в качестве Q_PROPERTY
   * как в C++, так и в QML.
   */

  /**
   * \brief Создает объект Envelope со значениями по умолчанию.
   * \details Значения по умолчанию: <tt>[50.0 м, 10.0 м]</tt>.
   */
  inline Envelope::Envelope()
    : m_altitude(50.0f)
    , m_width(10.0f)
  {}

  /**
   * \brief Создает объект Envelope с указанными значениями.
   * \param altitude - высота центра коридора от поверхности в м.
   * \param width - ширина коридора в м.
   */
  inline Envelope::Envelope(float altitude, float width)
    : m_altitude(altitude)
    , m_width(width)
  {}

  /**
   * \property Envelope::altitude
   * \brief Высота центра коридора от поверхности в м.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение                 <th>Запись                  <th>Оповещение
   * <tr><td><i>altitude</i>     <td><i>setAltitude</i>   <td><i>--</i>
   * </table>
   */
  inline float Envelope::altitude() const { return m_altitude; }
  inline void Envelope::setAltitude(float x) { m_altitude = x; }

  /**
   * \property Envelope::width
   * \brief Ширина коридора в м.
   * \details
   * <table>
   * <caption id="multi_row">Связанные функции</caption>
   * <tr><th>Чтение                 <th>Запись                  <th>Оповещение
   * <tr><td><i>width</i>   <td><i>setWidth</i> <td><i>--</i>
   * </table>
   */
  inline float Envelope::width() const { return m_width; }
  inline void Envelope::setWidth(float x) { m_width = x; }
} // ElevationChart