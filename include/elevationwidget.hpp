#pragma once

#include <QObject>
#include <QGeoPath>

class ElevationWidgetPrivate;
class ElevationWidget : public QObject
{
        Q_OBJECT
    public:
        explicit ElevationWidget(QObject *parent = nullptr);

        QGeoPath getGeopath();
        void setGeopath(QGeoPath path);

        void showIndexes(bool state);
        void setVelocity(float velocity);
        void setClimbRate(float rate);
        void setDescendRate(float rate);

        void setPallete(QString backgroundColor, QString foregroundColor, QString chartColor,
                        QString successColor, QString warningColor, QString errorColor);

    protected:
        ElevationWidgetPrivate* const d_ptr;

    private:
        Q_DECLARE_PRIVATE(ElevationWidget)

    signals:

};
