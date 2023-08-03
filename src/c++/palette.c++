//
// Created by whs31 on 03.08.23.
//

#include "palette.h"

Palette::Palette(QObject* parent)
  : QObject(parent)
  , m_background("black")
  , m_foreground("white")
  , m_overlay("gray")
  , m_accent("green")
  , m_warn("yellow")
  , m_error("red")
  , m_info("blue")
{}

/**
 * \property Palette::background
 * \brief Цвет фона.
 * \details По умолчанию равен <span style="background:#000000">#000000</span>.
 */
QColor Palette::background() const { return m_background; }
void Palette::setBackground(const QColor& x) {
  if(x == m_background)
    return;
  m_background = x;
  emit backgroundChanged();
}

/**
 * \property Palette::foreground
 * \brief Цвет, контрастный к фону.
 * \details По умолчанию равен <span style="background:#FFFFFF">#FFFFFF</span>.
 */
QColor Palette::foreground() const { return m_foreground; }
void Palette::setForeground(const QColor& x) {
  if(x == m_foreground)
    return;
  m_foreground = x;
  emit foregroundChanged();
}

/**
 * \property Palette::overlay
 * \brief Цвет, промежуточный между #background и #foreground.
 * \details По умолчанию равен <span style="background:#666666">#666666</span>.
 */
QColor Palette::overlay() const { return m_overlay; }
void Palette::setOverlay(const QColor& x) {
  if(x == m_overlay)
    return;
  m_overlay = x;
  emit overlayChanged();
}

/**
 * \property Palette::accent
 * \brief Основной акцентный цвет.
 * \details По умолчанию равен <span style="background:#00FF00">#00FF00</span>.
 */
QColor Palette::accent() const { return m_accent; }
void Palette::setAccent(const QColor& x) {
  if(x == m_accent)
    return;
  m_accent = x;
  emit accentChanged();
}

/**
 * \property Palette::warn
 * \brief Цвет предупреждений.
 * \details По умолчанию равен <span style="background:#FFFF00">#FFFF00</span>.
 */
QColor Palette::warn() const { return m_warn; }
void Palette::setWarn(const QColor& x) {
  if(x == m_warn)
    return;
  m_warn = x;
  emit warnChanged();
}

/**
 * \property Palette::error
 * \brief Цвет ошибок.
 * \details По умолчанию равен <span style="background:#FF0000">#FF0000</span>.
 */
QColor Palette::error() const { return m_error; }
void Palette::setError(const QColor& x) {
  if(x == m_error)
    return;
  m_error = x;
  emit errorChanged();
}

/**
 * \property Palette::info
 * \brief Цвет оповещений.
 * \details По умолчанию равен <span style="background:#0000FF">#0000FF</span>.
 */
QColor Palette::info() const { return m_info; }
void Palette::setInfo(const QColor& x) {
  if(x == m_info)
    return;
  m_info = x;
  emit infoChanged();
}
