//
// Created by whs31 on 03.08.23.
//

#pragma once
#include <QtCore/QObject>
#include <QtGui/QColor>

/**
 * \brief Палитра цветов.
 * \details Класс предоставляет набор цветов с сеттерами, геттерами и сигналами.
 *
 * Класс зарегистрирован в мета-системе и доступен для встраивания в типы QML.
 */
class Palette : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)
  Q_PROPERTY(QColor foreground READ foreground WRITE setForeground NOTIFY foregroundChanged)
  Q_PROPERTY(QColor overlay READ overlay WRITE setOverlay NOTIFY overlayChanged)
  Q_PROPERTY(QColor accent READ accent WRITE setAccent NOTIFY accentChanged)
  Q_PROPERTY(QColor warn READ warn WRITE setWarn NOTIFY warnChanged)
  Q_PROPERTY(QColor error READ error WRITE setError NOTIFY errorChanged)
  Q_PROPERTY(QColor info READ info WRITE setInfo NOTIFY infoChanged)

  public:
    explicit Palette(QObject* parent = nullptr);

    [[nodiscard]] QColor background() const; void setBackground(const QColor&);
    [[nodiscard]] QColor foreground() const; void setForeground(const QColor&);
    [[nodiscard]] QColor overlay() const;    void setOverlay(const QColor&);
    [[nodiscard]] QColor accent() const;     void setAccent(const QColor&);
    [[nodiscard]] QColor warn() const;       void setWarn(const QColor& color);
    [[nodiscard]] QColor error() const;      void setError(const QColor& color);
    [[nodiscard]] QColor info() const;       void setInfo(const QColor& color);

  signals:
    void backgroundChanged();
    void foregroundChanged();
    void overlayChanged();
    void accentChanged();
    void warnChanged();
    void errorChanged();
    void infoChanged();

  private:
    QColor m_background;
    QColor m_foreground;
    QColor m_overlay;
    QColor m_accent;
    QColor m_warn;
    QColor m_error;
    QColor m_info;
};

#include <QtCore/QMetaType>
Q_DECLARE_METATYPE(Palette*)