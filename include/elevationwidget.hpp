#pragma once

#include <QObject>

class ElevationWidgetPrivate;
class ElevationWidget : public QObject
{
    Q_OBJECT
    public:
        explicit ElevationWidget(QObject *parent = nullptr);

    protected:
        ElevationWidgetPrivate* const d_ptr;

    private:
        Q_DECLARE_PRIVATE(ElevationWidget)

    signals:

};
