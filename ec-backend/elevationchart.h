#ifndef ELEVATIONCHART_H
#define ELEVATIONCHART_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QPointF>
#include <QGeoPath>

namespace Elevation {
    class Elevation;
    //class Point;
    //class ElevationTools;
}

class ElevationChart : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoPath geopath READ geopath WRITE setGeopath NOTIFY geopathChanged)
    Q_PROPERTY(QList<QPointF> pathData READ pathData WRITE setPathData NOTIFY pathDataChanged)
    Q_PROPERTY(bool logging READ logging WRITE setLogging NOTIFY loggingChanged)

    Q_PROPERTY(qreal pixelWidth READ pixelWidth WRITE setPixelWidth NOTIFY pixelWidthChanged)
    Q_PROPERTY(qreal pixelHeight READ pixelHeight WRITE setPixelHeight NOTIFY pixelHeightChanged)
    Q_PROPERTY(qreal offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(qreal verticalStretch READ verticalStretch WRITE setVerticalStretch NOTIFY verticalStretchChanged)
    Q_PROPERTY(qreal zoomX READ zoomX WRITE setZoomX NOTIFY zoomXChanged)
    Q_PROPERTY(qreal realWidth READ realWidth WRITE setRealWidth NOTIFY realWidthChanged)
    Q_PROPERTY(qreal realHeight READ realHeight WRITE setRealHeight NOTIFY realHeightChanged)

    Q_PROPERTY(int scaleValueX READ scaleValueX WRITE setScaleValueX NOTIFY scaleValueXChanged)
    Q_PROPERTY(int scaleValueY READ scaleValueY WRITE setScaleValueY NOTIFY scaleValueYChanged)
    Q_PROPERTY(qreal scaleCountX READ scaleCountX WRITE setScaleCountX NOTIFY scaleCountXChanged)
    Q_PROPERTY(qreal scaleCountY READ scaleCountY WRITE setScaleCountY NOTIFY scaleCountYChanged)
    Q_PROPERTY(qreal scaleStepX READ scaleStepX WRITE setScaleStepX NOTIFY scaleStepXChanged)
    Q_PROPERTY(qreal scaleStepY READ scaleStepY WRITE setScaleStepY NOTIFY scaleStepYChanged)


public:
    explicit ElevationChart(QObject *parent = nullptr);

    Q_INVOKABLE float getMaxRange(void) const { return (float)axes.x.max; }
    Q_INVOKABLE QPointF iterateSimple(void);
    Q_INVOKABLE QPointF iterateOverRange(float rangeStart, float rangeStop);
    Q_INVOKABLE void changeFlightPointAltitude(int index, int direction);

public slots:
    //void intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point> &resultPath);

signals:
    void requestRedraw();


    void geopathChanged();
    void pathDataChanged();
    void loggingChanged();
    void pixelWidthChanged();
    void pixelHeightChanged();
    void offsetChanged();
    void verticalStretchChanged();
    void zoomXChanged();
    void zoomYChanged();
    void realHeightChanged();
    void realWidthChanged();
    void scaleValueXChanged();
    void scaleValueYChanged();
    void scaleCountXChanged();
    void scaleCountYChanged();
    void scaleStepXChanged();
    void scaleStepYChanged();

private:
    void update(bool vectorChanged = false);

    Elevation::Elevation* heightmapParser;
    //Elevation::ElevationTools* routeParser;

    QVector<QPointF> points;
    QGeoPath m_geopath;
    QList<QPointF> m_pathData;
    bool m_logging = false;
    struct Axes
    {
        struct Axis
        {
            int max = 0;
            int power = 0;
            int roundmax = 0;
            qreal pixelsize = 0;
            qreal zoom = 1;
            int scalevalue = 0;
            qreal scalecount = 0;
            qreal scalepixelwidth = 0;
        }; Axis x; Axis y;

        qreal offset = 20;
        qreal stretch = 1.5;
    }; Axes axes;

    struct Iterator
    {
        int simple = 0;
        int rangeMin = 0;
        int rangeMax = 0;
        int range = 0;
        bool rangeSet = false;
    }; Iterator iterator;

    QGeoPath geopath() const;                       void setGeopath(const QGeoPath &path);
    QList<QPointF> pathData() const;                void setPathData(QList<QPointF> data);
    bool logging() const;                           void setLogging(bool state);

    qreal pixelWidth() const;                       void setPixelWidth(qreal value);
    qreal pixelHeight() const;                      void setPixelHeight(qreal value);
    qreal offset() const;                           void setOffset(qreal value);
    qreal verticalStretch() const;                  void setVerticalStretch(qreal value);
    qreal zoomX() const;                            void setZoomX(qreal value);
    qreal realHeight() const;                       void setRealHeight(qreal value);
    qreal realWidth() const;                        void setRealWidth(qreal value);
    int scaleValueX() const;                        void setScaleValueX(int value);
    int scaleValueY() const;                        void setScaleValueY(int value);
    qreal scaleCountX() const;                      void setScaleCountX(qreal count);
    qreal scaleCountY() const;                      void setScaleCountY(qreal count);
    qreal scaleStepX() const;                       void setScaleStepX(qreal value);
    qreal scaleStepY() const;                       void setScaleStepY(qreal value);

};

#endif // ELEVATIONCHART_H
