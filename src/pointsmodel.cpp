#include "pointsmodel.hpp"
#include <QDebug>

using namespace Charts;

PointsModel::PointsModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_segfaultTimer(new QTimer(this))
{
    m_segfaultTimer.setInterval(20);
    m_segfaultTimer.setSingleShot(true);
}

int PointsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_points.size();
}

QVariant PointsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case PointRoles::Index:
            return index.row();
        case PointRoles::Distance:
            return m_points.at(index.row()).distance;
        case PointRoles::Altitude:
            return m_points.at(index.row()).altitude;
        case PointRoles::Valid:
            return m_points.at(index.row()).valid;
        case PointRoles::Intersects:
            return m_points.at(index.row()).intersects;
    }
    return QVariant();
}

QHash<int, QByteArray> PointsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PointRoles::Index] = "index";
    roles[PointRoles::Distance] = "distance";
    roles[PointRoles::Altitude] = "altitude";
    roles[PointRoles::Valid] = "valid";
    roles[PointRoles::Intersects] = "intersects";
    return roles;
}

void PointsModel::setPath(const std::vector<ChartPoint>& _points)
{
    if(rowCount() != 0)
        removePath();

    beginInsertRows(QModelIndex(), 0, _points.size() - 1);
    m_points = _points;
    endInsertRows();
}

void PointsModel::updatePath(const vector<ChartPoint>& _points)
{
    if(_points.empty() or _points.size() != m_points.size())
        return;
    m_points = _points;
    emit dataChanged(createIndex(0, 0), createIndex(rowCount(), 0));
}

void PointsModel::updatePoint(const int _index, const ChartPoint& _point)
{
    m_points[_index] = _point;
    emit dataChanged(createIndex(_index, 0), createIndex(_index,0));
}

void PointsModel::removePath()
{
    beginRemoveRows(QModelIndex(), 0 , rowCount() - 1);
    m_points.clear();
    endRemoveRows();
}

void PointsModel::changePointAltitude(const int _index, const float _altitude)
{
    if(m_segfaultTimer.isActive())
        return;

    m_points[_index].altitude = _altitude;
    emit pointChanged(_index);
    emit dataChanged(createIndex(_index, 0), createIndex(_index, 0));
    m_segfaultTimer.start(5);
}

ChartPoint PointsModel::getPoint(const int _index)
{
    return m_points.at(_index);
}
