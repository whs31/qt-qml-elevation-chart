//
// Created by whs31 on 03.08.23.
//

#pragma once
#include <QtCore/QObject>
#include <QtGui/QColor>

/**
 * \class QQuickItem
 * \extends QObject
 */

/**
 * \brief Пространство имен библиотеки Scenegraph.
 */
namespace SG
{
  /**
   * \brief Простейшая палитра цветов.
   * \details Класс предоставляет набор цветов с сеттерами, геттерами и сигналами.
   * Доступны следующие цвета:
   * - <span style="background:#000000">цвет заднего плана</span>
   * - <span style="background:#FFFFFF">цвет переднего плана</span>
   * - <span style="background:#666666">промежуточный цвет</span>
   * - <span style="background:#777777">второй промежуточный цвет</span>
   * - <span style="background:#00FF00">акцентный цвет</span>
   * - <span style="background:#FFFF00">цвет предупреждений</span>
   * - <span style="background:#FF0000">цвет ошибок</span>
   * - <span style="background:#0000FF">цвет оповещений</span>
   *
   * Класс зарегистрирован в мета-системе и доступен для встраивания в типы QML.
   */
  class BasicPalette
  {
    Q_GADGET
    Q_PROPERTY(QColor background READ background WRITE setBackground)
    Q_PROPERTY(QColor foreground READ foreground WRITE setForeground)
    Q_PROPERTY(QColor overlay READ overlay WRITE setOverlay)
    Q_PROPERTY(QColor overlay2 READ overlay2 WRITE setOverlay2)
    Q_PROPERTY(QColor accent READ accent WRITE setAccent)
    Q_PROPERTY(QColor warn READ warn WRITE setWarn)
    Q_PROPERTY(QColor error READ error WRITE setError)
    Q_PROPERTY(QColor info READ info WRITE setInfo)

    public:
      BasicPalette();

      [[nodiscard]] QColor background() const; void setBackground(const QColor&);
      [[nodiscard]] QColor foreground() const; void setForeground(const QColor&);
      [[nodiscard]] QColor overlay() const; void setOverlay(const QColor&);
      [[nodiscard]] QColor overlay2() const; void setOverlay2(const QColor&);
      [[nodiscard]] QColor accent() const; void setAccent(const QColor&);
      [[nodiscard]] QColor warn() const; void setWarn(const QColor& color);
      [[nodiscard]] QColor error() const; void setError(const QColor& color);
      [[nodiscard]] QColor info() const; void setInfo(const QColor& color);

    private:
      QColor m_background;
      QColor m_foreground;
      QColor m_overlay;
      QColor m_overlay2;
      QColor m_accent;
      QColor m_warn;
      QColor m_error;
      QColor m_info;
  };
} // SG

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(SG::BasicPalette)
