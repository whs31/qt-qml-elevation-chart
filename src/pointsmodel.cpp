#include "pointsmodel.hpp"

using namespace Charts;

PointsModel::PointsModel(QObject *parent)
    : QAbstractListModel(parent)
{
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
    beginResetModel();     //если не работает, закоментировать и поменять на нижнее
    m_points = _points;    //если не работает, закоментировать и поменять на нижнее
    endResetModel();       //если не работает, закоментировать и поменять на нижнее

//    if(rowCount() != 0){
//        removePath();
//    }
//    beginInsertRows(QModelIndex(), 0, _points.size());
//    m_points = _points;
//    endInsertRows();

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

ChartPoint PointsModel::getPoint(const int _index)
{
    return m_points.at(_index);
}

