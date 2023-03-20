#include "elevationwidget.hpp"
#include "elevationwidget_p.hpp"

#include <qqml.h>
#include <cmath>
#include "Elevation/elevation.h"
#include <QMetaType>
//#include <QDir>

ElevationWidget::ElevationWidget(QObject *parent)
    : QObject{parent}
    , d_ptr(new ElevationWidgetPrivate(this))
{
    //Q_INIT_RESOURCE(qplotjs);
    qmlRegisterSingletonInstance("ElevationWidgetImpl", 1, 0, "Impl", d_ptr);
}

QGeoPath ElevationWidget::getGeopath()
{
    Q_D(ElevationWidget);
    return d->geopath;
}

void ElevationWidget::setGeopath(const QGeoPath& path)
{
    Q_D(ElevationWidget);
    if(path == d->geopath and not path.isEmpty())
        return;
    d->geopath = path;
    d->recalculateWithGeopathChanged();
}

bool ElevationWidget::isPathMatchingMetrics()
{
    Q_D(ElevationWidget);
    return d->m_isMatchingMetrics;
}

QGeoPath ElevationWidget::applyMetricsCorrection()
{
    Q_D(ElevationWidget);
    if(isPathMatchingMetrics())
    {
        qWarning() << "<qplot> Пути совпадают, изменения пути не будут применены";
        return QGeoPath();
    }

    QGeoPath returnPath = d->geopath;
    for(size_t i = 0; i < d->geopath.path().length(); i++)
    {
        if(i < d->metricsCorrectedGeopath.path().length())
            returnPath.replaceCoordinate(i, d->metricsCorrectedGeopath.path()[i]);
    }

    d->metricsCorrectedGeopath = returnPath;
    setGeopath(returnPath);

    qInfo() << "<qplot> Изменения применены";
    return getGeopath();
}

void ElevationWidget::showIndexes(bool state)
{
    Q_D(ElevationWidget);
    d->setShowIndex(state);
}

void ElevationWidget::setVelocity(float velocity)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.velocity = velocity;
    d->recalculate();
//    d->recalculateBound(true);
}

void ElevationWidget::setClimbRate(float rate)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.climbRate = rate;
    d->recalculate();
//    d->recalculateBound(true);
}

void ElevationWidget::setDescendRate(float rate)
{
    Q_D(ElevationWidget);
    d->aircraftMetrics.descendRate = rate;
    d->recalculate();
//    d->recalculateBound(true);
}

void ElevationWidget::setBoundHeight(float height)
{
    Q_D(ElevationWidget);
    //d->bound.height = height;
//    d->recalculateBound(true);
}

void ElevationWidget::setBoundWidth(float width)
{
    Q_D(ElevationWidget);
    //d->bound.width = width;
//    d->recalculateBound(true);
}

bool ElevationWidget::isIntersecting(void)
{
    Q_D(ElevationWidget);
    return d->m_isIntersecting;
}

bool ElevationWidget::isValidPath()
{
    Q_D(ElevationWidget);
    return d->fileIntegrity();
}

void ElevationWidget::setPallete(QString backgroundColor, QString foregroundColor, QString chartColor,
                                 QString successColor, QString warningColor, QString errorColor)
{
    Q_D(ElevationWidget);
    d->setColors({ backgroundColor, foregroundColor, chartColor, successColor, warningColor, errorColor });

}

/*
███████████████████████████          🔒 PRIVATE IMPLEMENTATION 🔒          ███████████████████████████████──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────f
*/
ElevationWidgetPrivate::ElevationWidgetPrivate(ElevationWidget* parent)
    : QObject{parent}
    , q_ptr(parent)
{
    // submodule types
    heightmapParser = new Elevation::Elevation(this);
    routeParser = new Elevation::ElevationTools(this);

    qRegisterMetaType<QVector<Elevation::Point>>("QVector<Point>");
    qRegisterMetaType<Elevation::RouteAndElevationProfiles>("RouteAndElevationProfiles");
    connect(routeParser, &Elevation::ElevationTools::progressTestRouteIntersectGround,
            this, &ElevationWidgetPrivate::intersectCalculationFinished);
//    connect(routeParser, &Elevation::ElevationTools::progressBuildRouteAndElevationProfiles,
//            this, &ElevationWidgetPrivate::boundCalculationFinished);

//    routeTimer = new QTimer(this);
//    routeTimer->setInterval(1);
//    routeTimer->setSingleShot(true);
    //recalculateWithGeopathChanged(); <- geopath is empty = draw blank screen
}

QPointF ElevationWidgetPrivate::toPixel(const QPointF& point)
{
    return QPointF(point.x() * layout.width / axis.x.maxValue,
                   layout.height - point.y() * layout.height / (axis.y.maxValue * axis.stretch));
}

// █ calculate axes and scales
void ElevationWidgetPrivate::recalculate(bool emitFlag)
{
    if(profile().isEmpty()){
        calculatePath();
        calculateCorrectedPath();
        setKeyValues({});
        emit requestAll();
        return;
    }

    float _emit_check = axis.y.maxValue;
    bool _emit_checkflag = false;

    iterator.range = 0;
    iterator.rangeSet = false;

    axis.x.maxValue = profile().last().x();
    axis.x.roundMaxValue = 0;
    axis.y.maxValue = 0;
    axis.y.roundMaxValue = 0;

    // проверка на целостность профиля высот
    setFileIntegrity(true);
    for(QPointF point : profile())
    {
        if(point.y() > 32000)
        {
            qWarning() << "<qplot> Some elevation profiles are missing.";
            setFileIntegrity(false);
            return;
        }
    }

    for(QGeoCoordinate coord : geopath.path())
    {
        if(coord.altitude() > axis.y.maxValue)
            axis.y.maxValue = coord.altitude();
    }
    if(_emit_check != axis.y.maxValue)
        _emit_checkflag = true;

    int _power_x = axis.x.maxValue > 0 ? (int) log10 ((float) axis.x.maxValue) : 1;
    int _power_y = axis.y.maxValue > 0 ? (int) log10 ((float) axis.y.maxValue) : 1;
    while(axis.x.roundMaxValue < axis.x.maxValue)
    {
        axis.x.roundMaxValue += pow(10, _power_x);
    }
    while(axis.y.roundMaxValue < axis.y.maxValue)
    {
        axis.y.roundMaxValue += pow(10, _power_y);
    }
    axis.x.scaleValue = pow(10, _power_x);
    axis.y.scaleValue = pow(10, _power_y);
    axis.x.scaleCount = (float)axis.x.maxValue / (float)axis.x.scaleValue;
    axis.y.scaleCount = (float)axis.y.maxValue * axis.stretch / (float)axis.y.scaleValue;
    axis.x.scalePixelSize = layout.width / axis.x.scaleCount;
    axis.y.scalePixelSize = layout.height / axis.y.scaleCount;
    setKeyValues({  axis.stretch,                                                                               // 0
                    axis.x.maxValue, axis.y.maxValue,                                                           // 1-2
                    axis.x.roundMaxValue, axis.y.roundMaxValue,                                                 // 3-4
                    static_cast<float>(axis.x.scaleValue), static_cast<float>(axis.y.scaleValue),               // 5-6
                    axis.x.scaleCount, axis.y.scaleCount,                                                       // 7-8
                    axis.x.scalePixelSize, axis.y.scalePixelSize  });                                           // 9-10

    calculatePath();
    calculateCorrectedPath();

    routeParser->testRouteIntersectGround(geopath);
    //recalculateBound(emitFlag);

    if(emitFlag or profile().isEmpty() or _emit_checkflag)
        emit requestAll();
    else
        emit requestPath();
}


// █ build profile
void ElevationWidgetPrivate::recalculateWithGeopathChanged()
{
    if(geopath.isEmpty())
    {
        setValid(false);
        qWarning() << "<qplot> Empty geopath.";
        return;
    }
    setValid(true);
    setProfile(heightmapParser->buildGroundProfileForChart(geopath));
    recalculate(true);
}

void ElevationWidgetPrivate::calculatePath()
{
    auto data = QList<QPointF>();
    float previous_distance = 0;
    for(size_t i = 0; i < geopath.path().length(); i++)
    {
        float delta_s = 0;
        if(i > 0)
            delta_s = geopath.path()[i].distanceTo(geopath.path()[i-1]);
        previous_distance += delta_s;
        QPointF point(previous_distance, geopath.path()[i].altitude());
        data.append(toPixel(point));
    }
    setPath(data);
}

void ElevationWidgetPrivate::calculateCorrectedPath()
{
    QGeoPath correctPath;
    if (geopath.isEmpty()){
        calculateCorrectedPathForUI(correctPath);
        return;
    }

    // При обновлении маршута вызывается дважды хз почему       @BUG: проверить

    correctPath.addCoordinate(geopath.path()[0]);
    m_isMatchingMetrics = true;
    for(size_t i = 1; i < geopath.path().length(); i++)
    {
        float delta_x = geopath.path()[i].distanceTo(geopath.path()[i-1]);
        float delta_y = geopath.path()[i].altitude() - correctPath.path()[i-1].altitude();
        const float delta_y_min = aircraftMetrics.descendRate * delta_x / aircraftMetrics.velocity;
        const float delta_y_max = aircraftMetrics.climbRate * delta_x / aircraftMetrics.velocity;
        QGeoCoordinate coordinate = QGeoCoordinate(geopath.path()[i]);

        // correct case
        if((delta_y >= 0 and delta_y < delta_y_max) or (delta_y <= 0 and abs(delta_y) < delta_y_min))
        {
            correctPath.addCoordinate(geopath.path()[i]);
            continue;
        }

        // climbs too fast
        else if(delta_y > 0 and delta_y > delta_y_max)
        {
            coordinate.setAltitude(correctPath.path()[i-1].altitude() + delta_y_max);
            m_isMatchingMetrics = false;
        }
        // descends too fast
        else if(delta_y < 0 and abs(delta_y) > delta_y_min)
        {
            coordinate.setAltitude(correctPath.path()[i-1].altitude() - delta_y_min);
            m_isMatchingMetrics = false;
        }
        correctPath.addCoordinate(coordinate);

        if(not m_isMatchingMetrics)
            metricsCorrectedGeopath = correctPath;
    }
    calculateCorrectedPathForUI(correctPath);

}

void ElevationWidgetPrivate::calculateCorrectedPathForUI(QGeoPath c_geopath)
{
    QList<QPointF> data;
    float previous_distance = 0;
    for(size_t i = 0; i < c_geopath.path().length(); i++)
    {
        QPointF point;
        float delta_s = 0;
        point.setY(layout.height - c_geopath.path()[i].altitude() * layout.height / (axis.y.maxValue * axis.stretch));
        if(i > 0)
            delta_s = c_geopath.path()[i].distanceTo(c_geopath.path()[i-1]);
        previous_distance += delta_s;
        point.setX(previous_distance * layout.width / axis.x.maxValue);
        data.append(point);
    }
    setCorrectedPath(data);
}

void ElevationWidgetPrivate::intersectCalculationFinished(quint8 progress, const QVector<Elevation::Point>& resultPath)
{
    QList<QPointF> _intersectList;
    // check for first & last point lies inside ground:
    bool _first_point_in_ground = false;
    bool _last_point_in_ground = false;
    if((float)routeParser->elevationGeoPoint(geopath.path().first().latitude(), geopath.path().first().longitude()) > geopath.path().first().altitude())
        _first_point_in_ground = true;
    if((float)routeParser->elevationGeoPoint(geopath.path().last().latitude(), geopath.path().last().longitude()) > geopath.path().last().altitude())
        _last_point_in_ground = true;

    if(_first_point_in_ground)
        _intersectList.append(QPointF(0, layout.height - geopath.path().first().altitude() * layout.height / (axis.y.maxValue * axis.stretch)));

    // эта херня будет отвечать за то, лежит ли любая часть маршрута внутри рельефа)
    // bool _intersects_flag = false;
    for(size_t i = 0; i < resultPath.length(); i++)
    {
        if(resultPath[i].isBase())
            continue;
        QPointF _point(resultPath[i].distance(), resultPath[i].altitude());
        _intersectList.append(toPixel(_point));
        //@FIXME: заменить на {not m_isIntersecting}, когда вова перестанет душить
        if(!m_isIntersecting)
            m_isIntersecting = true;
    }
    Q_Q(ElevationWidget);
    if(m_isIntersecting)
        emit(q->intersectingStateChanged());

    if(_last_point_in_ground)
        _intersectList.append(QPointF(layout.width, layout.height - geopath.path().last().altitude() * layout.height / (axis.y.maxValue * axis.stretch)));
    setIntersections(_intersectList);
    emit requestIntersects();
}

void ElevationWidgetPrivate::resize(float w, float h, float zoom_w, float zoom_h)
{
    const float RIGHT_OFFSET = 10;
    if(w == layout.width and h == layout.height and
       zoom_w == layout.horizontal_zoom and zoom_h == layout.vertical_zoom)
        return;
    layout.width = w - RIGHT_OFFSET;
    layout.height = h;
    layout.horizontal_zoom = zoom_w;
    layout.vertical_zoom = zoom_h;
    if(not geopath.isEmpty() and layout.width * layout.height > 0)
    {
        recalculate();
        //recalculateBound();
    }
}

QPointF ElevationWidgetPrivate::iterateOverRange(float rangeStart, float rangeStop)
{
    if(geopath.isEmpty())
        return QPointF(-1, -1);
    if(not iterator.rangeSet)
    {
        iterator.rangeMin = -1;
        iterator.rangeMax = -1;
        iterator.range = 0;
        if(rangeStart == 0)
            iterator.rangeMin = 0;
        if(rangeStop == 1)
            iterator.rangeMax = m_profile.length() - 1;
        if(rangeStart != 0 and rangeStop != 1)
        {
            int lval1 = rangeStart * axis.x.maxValue;
            int lval2 = rangeStop * axis.x.maxValue;
            bool _rangeMinFlag = false;
            for(int i = 0; i < m_profile.length(); i++)
            {
                int rval = m_profile[i].x();
                if(lval1 < rval and not _rangeMinFlag)
                {
                    iterator.rangeMin = i > 0 ? i - 1 : 0;
                    _rangeMinFlag = true;
                }
                if(lval2 >= rval)
                    iterator.rangeMax = i < m_profile.length() - 1 ? i + 1 : i;
            }
        }
        if (iterator.rangeMax == -1) {
            qDebug() <<"<qplot> range assignment\t" << "upper range not found in vector";
            return QPointF(-1, -1);
        }
        Q_ASSERT_X(iterator.rangeMax < m_profile.length(), "<qplot> bounds", "max range is out of bounds");
        Q_ASSERT_X(iterator.rangeMin >= 0, "<qplot> bounds", "min range is lower than 0");
        iterator.rangeSet = true;
        iterator.range = iterator.rangeMin;
    }
    iterator.range++;
    if(iterator.range < iterator.rangeMax)
    {
        return QPointF(m_profile[iterator.range].x() * layout.width / axis.x.maxValue,
                m_profile[iterator.range].y() * layout.height / (axis.y.maxValue * axis.stretch));
    } else {
        iterator.range = 0;
        iterator.rangeSet = false;
        return QPointF(-1, -1);
    }
}

void ElevationWidgetPrivate::changeFlightPointAltitude(int index, qreal delta)
{
    QGeoCoordinate coord = geopath.coordinateAt(index);
    coord.setAltitude(coord.altitude() + delta * (coord.altitude() / axis.y.roundMaxValue * .2 + .01));
    if(coord.altitude() <= 0)
        coord.setAltitude(0);
    if(coord.altitude() >= 15'000)
    {
        coord.setAltitude(15000);
        qInfo() << "?)";
    }
    geopath.replaceCoordinate(index, coord);
    recalculate();

    Q_Q(ElevationWidget);
    emit(q->geopathChanged());
}

QList<QString> ElevationWidgetPrivate::colors() const { return m_colors; }
void ElevationWidgetPrivate::setColors(const QList<QString>& list) {
    if (m_colors == list) return;
    m_colors = list;
    emit colorsChanged();
}
QVector<QPointF> ElevationWidgetPrivate::profile() const { return m_profile; }
void ElevationWidgetPrivate::setProfile(const QVector<QPointF>& vec) {
    if (m_profile == vec) return;
    m_profile = vec;
    emit profileChanged();
    recalculateWithGeopathChanged();
}
QList<QPointF> ElevationWidgetPrivate::path() const { return m_path; }
void ElevationWidgetPrivate::setPath(const QList<QPointF>& list) {
    if (m_path == list) return;
    m_path = list;
    emit pathChanged();
}
QList<QPointF> ElevationWidgetPrivate::correctedPath() const { return m_correctedPath; }
void ElevationWidgetPrivate::setCorrectedPath(const QList<QPointF>& list) {
    if (m_correctedPath == list) return;
    m_correctedPath = list;
    emit correctedPathChanged();
}
QList<float> ElevationWidgetPrivate::keyValues() const { return m_keyValues; }
void ElevationWidgetPrivate::setKeyValues(const QList<float>& values) {
    if (m_keyValues == values) return;
    m_keyValues = values;
    emit keyValuesChanged();
}
bool ElevationWidgetPrivate::showIndex() const { return input.showIndex; }
void ElevationWidgetPrivate::setShowIndex(bool state) {
    if (input.showIndex == state) return;
    input.showIndex = state;
    emit showIndexChanged();
}
QList<QPointF> ElevationWidgetPrivate::intersections() const { return m_intersections; }
void ElevationWidgetPrivate::setIntersections(const QList<QPointF>& list) {
    if (m_intersections == list) return;
    m_intersections = list;
    emit intersectionsChanged();
}
bool ElevationWidgetPrivate::fileIntegrity() const { return m_fileIntegrity; }
void ElevationWidgetPrivate::setFileIntegrity(bool state) {
    if (m_fileIntegrity == state) return;
    m_fileIntegrity = state;
    emit fileIntegrityChanged();
}
bool ElevationWidgetPrivate::valid() const { return m_valid; }
void ElevationWidgetPrivate::setValid(bool state) {
    if (m_valid == state) return;
    m_valid = state;
    emit validChanged();
}

