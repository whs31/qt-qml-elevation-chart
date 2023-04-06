#ifndef POINTSMODEL_H
#define POINTSMODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QPointF>


struct CustomPoint{
    size_t index = -1;
    float distance = 0;
    float altitude = 0;
    bool valid = true;
    bool intersects = false;
};

class PointsModel : public QAbstractListModel
{
    Q_OBJECT
    enum PointRoles{
        index = Qt::UserRole + 1,
        distance,
        altitude,
        valid,
        intersects
    };

public:
    explicit PointsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setPath(const std::vector<CustomPoint> &_points);
    void updatePoint(const int _index, const CustomPoint &_point);
    void removePath();

    CustomPoint getPoint(const int _index);


private:
    std::vector<CustomPoint> m_points;
};

#endif // POINTSMODEL_H
