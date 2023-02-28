#ifndef ELEVATIONCHART_H
#define ELEVATIONCHART_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QPointF>
#include <QGeoPath>
#include "macro.hpp"
#include "impl/fpscounter.hpp"

// по неизвестным науке причинам я не могу сделать forward declaration этого класса =(
#include "RouteTools/elevationtools.h"


namespace Elevation {
    class Elevation;
}

class ElevationChart : public QObject
{
    Q_OBJECT

public:
    explicit ElevationChart(QObject *parent = nullptr);

    Q_INVOKABLE QPointF iterateSimple(void);
    Q_INVOKABLE QPointF iterateOverRange(float rangeStart, float rangeStop);
    Q_INVOKABLE void changeFlightPointAltitude(int index, qreal delta);
    Q_INVOKABLE void updateProfile(void);

public slots:
    void intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point> &resultPath);

signals:
    void requestRedraw();
    void requestRedrawPath();
    void requestRedrawIntersects();

private:
    //class FPSCounter;
    Q_PROPERTY(FPSCounter* fpsCounter MEMBER fpsCounterImpl CONSTANT)
    FPSCounter* fpsCounterImpl;
    Elevation::Elevation* heightmapParser;
    Elevation::ElevationTools* routeParser;

private:
    void update(bool vectorChanged = false);

    PROPERTY_NOCOMPARE_SET_ARGS(QGeoPath, geopath)
        if(pixelWidth() > 0)
            update(false);
        }
        EMIT geopathChanged END
    PROPERTY(QList<QPointF>, pathData) EMIT pathDataChanged END
    PROPERTY(QList<QPointF>, fixedPathData) EMIT fixedPathDataChanged END
    PROPERTY(QList<QPointF>, intersectList) EMIT intersectListChanged END
    PROPERTY(QList<bool>, pathErrorList) EMIT pathErrorListChanged END
    PROPERTY(QList<float>, pathErrorValueList) EMIT pathErrorValueListChanged END

    PROPERTY(bool, logging) EMIT loggingChanged END

    PROPERTY_SET_ARGS(qreal, pixelWidth)
        if(!geopath().isEmpty() && pixelWidth() > 0)
            update(false);
        }
        EMIT pixelWidthChanged END
    PROPERTY_SET_ARGS(qreal, pixelHeight)
        if(!geopath().isEmpty() && pixelWidth() > 0)
            update(false);
        }
        EMIT pixelHeightChanged END
    PROPERTY(qreal, realWidth) EMIT realWidthChanged END
    PROPERTY(qreal, realHeight) EMIT realHeightChanged END
    PROPERTY(qreal, zoomX) EMIT zoomXChanged END
    PROPERTY(int, scaleValueX) EMIT scaleValueXChanged END
    PROPERTY(int, scaleValueY) EMIT scaleValueYChanged END
    PROPERTY(qreal, scaleCountX) EMIT scaleCountXChanged END
    PROPERTY(qreal, scaleCountY) EMIT scaleCountYChanged END
    PROPERTY(qreal, scaleStepX) EMIT scaleStepXChanged END
    PROPERTY(qreal, scaleStepY) EMIT scaleStepYChanged END
    PROPERTY(qreal, offset) EMIT offsetChanged END
    PROPERTY(qreal, verticalStretch) EMIT verticalStretchChanged END

    PROPERTY_SET_ARGS(qreal, variometerHV)
        if(!geopath().isEmpty() && pixelWidth() > 0)
            update(false);
        }
        EMIT variometerHVChanged END
    PROPERTY_SET_ARGS(qreal, variometerROC)
        if(!geopath().isEmpty() && pixelWidth() > 0)
            update(false);
        }
        EMIT variometerROCChanged END
    PROPERTY_SET_ARGS(qreal, variometerROD)
        if(!geopath().isEmpty() && pixelWidth() > 0)
            update(false);
        }
        EMIT variometerRODChanged END

    int m_powerX = 0;
    int m_powerY = 0;
    int m_roundmaxX = 0;
    int m_roundmaxY = 0;

    QVector<QPointF> points;
    struct Iterator
    {
        int simple = 0;
        int rangeMin = 0;
        int rangeMax = 0;
        int range = 0;
        bool rangeSet = false;
    }; Iterator iterator;
};

#endif // ELEVATIONCHART_H
