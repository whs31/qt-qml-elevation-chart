//
// Created by whs31 on 03.08.23.
//

#include "basicpalette.h"

namespace SG
{
  BasicPalette::BasicPalette()
    : m_background("black")
    , m_foreground("white")
    , m_overlay("gray")
    , m_accent("green")
    , m_warn("yellow")
    , m_error("red")
    , m_info("blue")
  {}

  /**
   * \property BasicPalette::background
   * \brief Цвет фона.
   * \details По умолчанию равен <span style="background:#000000">#000000</span>.
   */
  QColor BasicPalette::background() const { return m_background; }
  void BasicPalette::setBackground(const QColor& x) { m_background = x; }

  /**
   * \property BasicPalette::foreground
   * \brief Цвет, контрастный к фону.
   * \details По умолчанию равен <span style="background:#FFFFFF">#FFFFFF</span>.
   */
  QColor BasicPalette::foreground() const { return m_foreground; }
  void BasicPalette::setForeground(const QColor& x) { m_foreground = x; }

  /**
   * \property BasicPalette::overlay
   * \brief Цвет, промежуточный между #background и #foreground.
   * \details По умолчанию равен <span style="background:#666666">#666666</span>.
   */
  QColor BasicPalette::overlay() const { return m_overlay; }
  void BasicPalette::setOverlay(const QColor& x) { m_overlay = x; }

  /**
   * \property BasicPalette::accent
   * \brief Основной акцентный цвет.
   * \details По умолчанию равен <span style="background:#00FF00">#00FF00</span>.
   */
  QColor BasicPalette::accent() const { return m_accent; }
  void BasicPalette::setAccent(const QColor& x) { m_accent = x; }

  /**
   * \property BasicPalette::warn
   * \brief Цвет предупреждений.
   * \details По умолчанию равен <span style="background:#FFFF00">#FFFF00</span>.
   */
  QColor BasicPalette::warn() const { return m_warn; }
  void BasicPalette::setWarn(const QColor& x) { m_warn = x; }

  /**
   * \property BasicPalette::error
   * \brief Цвет ошибок.
   * \details По умолчанию равен <span style="background:#FF0000">#FF0000</span>.
   */
  QColor BasicPalette::error() const { return m_error; }
  void BasicPalette::setError(const QColor& x) { m_error = x; }

  /**
   * \property BasicPalette::info
   * \brief Цвет оповещений.
   * \details По умолчанию равен <span style="background:#0000FF">#0000FF</span>.
   */
  QColor BasicPalette::info() const { return m_info; }
  void BasicPalette::setInfo(const QColor& x) { m_info = x; }
}