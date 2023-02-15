#ifndef ELEVATIONCHART_H
#define ELEVATIONCHART_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QPointF>
#include <QGeoPath>
#include "macro.hpp"

#include "RouteTools/elevationtools.h" // по неизвестным науке причинам я не могу сделать
                                       // forward declaration этого класса =(

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
    Q_PROPERTY(QList<QPointF> intersectList READ intersectList WRITE setIntersectList NOTIFY intersectListChanged)
    Q_PROPERTY(QList<bool> pathErrorList READ pathErrorList WRITE setPathErrorList NOTIFY pathErrorListChanged)
    Q_PROPERTY(QList<float> pathErrorValueList READ pathErrorValueList WRITE setPathErrorValueList NOTIFY pathErrorValueListChanged)
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

    Q_PROPERTY(qreal variometerHV READ variometerHV WRITE setVariometerHV NOTIFY variometerHVChanged)
    Q_PROPERTY(qreal variometerROC READ variometerROC WRITE setVariometerROC NOTIFY variometerROCChanged)
    Q_PROPERTY(qreal variometerROD READ variometerROD WRITE setVariometerROD NOTIFY variometerRODChanged)


public:
    explicit ElevationChart(QObject *parent = nullptr);

    Q_INVOKABLE float getMaxRange(void) const { return (float)axes.x.max; }
    Q_INVOKABLE QPointF iterateSimple(void);
    Q_INVOKABLE QPointF iterateOverRange(float rangeStart, float rangeStop);
    Q_INVOKABLE void changeFlightPointAltitude(int index, qreal delta);

public slots:
    void intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point> &resultPath);

signals:
    void requestRedraw();

    void geopathChanged();              void pathDataChanged();         void pathErrorListChanged();
    void loggingChanged();              void pixelWidthChanged();       void pixelHeightChanged();
    void offsetChanged();               void verticalStretchChanged();  void zoomXChanged();
    void zoomYChanged();                void realHeightChanged();       void realWidthChanged();
    void scaleValueXChanged();          void scaleValueYChanged();      void scaleCountXChanged();
    void scaleCountYChanged();          void scaleStepXChanged();       void scaleStepYChanged();
    void variometerHVChanged();         void variometerROCChanged();    void variometerRODChanged();
    void pathErrorValueListChanged();

    void intersectListChanged();

private:
    void update(bool vectorChanged = false);

    Elevation::Elevation* heightmapParser;
    Elevation::ElevationTools* routeParser;
    QVector<QPointF> points;
    struct Iterator
    {
        int simple = 0;
        int rangeMin = 0;
        int rangeMax = 0;
        int range = 0;
        bool rangeSet = false;
    }; Iterator iterator;

    QGeoPath m_geopath;                 QPR_GET(QGeoPath, geopath)                  QPR_SET_NOCHECK_U(QGeoPath, geopath, setGeopath)
    QList<QPointF> m_pathData;          QPR_GET(QList<QPointF>, pathData)           QPR_SET_NOCHECK(QList<QPointF>, pathData, setPathData)
    QPR(QList<QPointF>, intersectList, setIntersectList)
    QPR(QList<bool>, pathErrorList, setPathErrorList)
    QPR(QList<float>, pathErrorValueList, setPathErrorValueList)

    QPR(bool, logging, setLogging)

    struct Axes
    {
        struct Axis
        {
            qreal pixelsize = 0;
            int max = 0;
            int power = 0;
            int roundmax = 0;
            qreal zoom = 1;
            int scalevalue = 0;
            qreal scalecount = 0;
            qreal scalestep = 0;
        }; Axis x; Axis y;

        qreal offset = 20;
        qreal stretch = 1.5;
    }; Axes axes;
    QPR_M_UPDATE(qreal, pixelWidth, setPixelWidth, axes.x.pixelsize) QPR_M_UPDATE(qreal, pixelHeight, setPixelHeight, axes.y.pixelsize)
    /* x.max */         QPR_GET_M(qreal, realWidth, axes.x.max)      QPR_SET_M_NOCHECK(qreal, realWidth, setRealWidth, axes.x.max)
    /* y.max */         QPR_GET_M(qreal, realHeight, axes.y.max)     QPR_SET_M_NOCHECK(qreal, realHeight, setRealHeight, axes.y.max)
    QPR_M(qreal, zoomX, setZoomX, axes.x.zoom)
    QPR_M(int, scaleValueX, setScaleValueX, axes.x.scalevalue)       QPR_M(int, scaleValueY, setScaleValueY, axes.y.scalevalue)
    QPR_M(qreal, scaleCountX, setScaleCountX, axes.x.scalecount)     QPR_M(qreal, scaleCountY, setScaleCountY, axes.y.scalecount)
    QPR_M(qreal, scaleStepX, setScaleStepX, axes.x.scalestep)        QPR_M(qreal, scaleStepY, setScaleStepY, axes.y.scalestep)
    QPR_M(qreal, offset, setOffset, axes.offset)
    QPR_M(qreal, verticalStretch, setVerticalStretch, axes.stretch)

    struct Variometer
    {
        qreal hV = 0;   // horizontal velocity
        qreal RoC = 0;  // rate of climb
        qreal RoD = 0;  // rate of descend
    }; Variometer variometer;
    QPR_M_UPDATE(qreal, variometerHV, setVariometerHV, variometer.hV)
    QPR_M_UPDATE(qreal, variometerROC, setVariometerROC, variometer.RoC)
    QPR_M_UPDATE(qreal, variometerROD, setVariometerROD, variometer.RoD)
};

#endif // ELEVATIONCHART_H
