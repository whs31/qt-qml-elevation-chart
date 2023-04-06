#include "pointsmodel.hpp"

PointsModel::PointsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}



int PointsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()){
        return 0;
    }
    return m_points.size();
}

QVariant PointsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    switch (role) {
    case PointRoles::index:
        return index.row();
    case PointRoles::distance:
        return m_points.at(index.row()).distance;
    case PointRoles::altitude:
        return m_points.at(index.row()).altitude;
    case PointRoles::valid:
        return m_points.at(index.row()).valid;
    case PointRoles::intersects:
        return m_points.at(index.row()).intersects;
    }
    return QVariant();
}

QHash<int, QByteArray> PointsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PointRoles::index] = "index";
    roles[PointRoles::distance] = "distance";
    roles[PointRoles::altitude] = "altitude";
    roles[PointRoles::valid] = "valid";
    roles[PointRoles::intersects] = "intersects";
    return roles;

}

void PointsModel::setPath(const std::vector<CustomPoint> &_points)
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

void PointsModel::updatePoint(const int _index, const CustomPoint &_point)
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

CustomPoint PointsModel::getPoint(const int _index)
{
    return m_points.at(_index);
}

