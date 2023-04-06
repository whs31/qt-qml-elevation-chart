#pragma once

#include <QAbstractListModel>
#include <vector>
#include <QPointF>

namespace Charts
{
    struct ChartPoint
    {
        size_t index = -1;
        float distance = 0;
        float altitude = 0;
        bool valid = true;
        bool intersects = false;
    };

    class PointsModel : public QAbstractListModel
    {
        Q_OBJECT
        std::vector<ChartPoint> m_points;

        public:
            enum PointRoles
            {
                Index = Qt::UserRole + 1,
                Distance,
                Altitude,
                Valid,
                Intersects
            };

            explicit PointsModel(QObject *parent = nullptr);

            int rowCount(const QModelIndex &parent = QModelIndex()) const override;
            QVariant data(const QModelIndex &index, int role) const override;
            QHash<int, QByteArray> roleNames() const override;

            void setPath(const std::vector<ChartPoint> &_points);
            void updatePoint(const int _index, const ChartPoint &_point);
            void removePath();

            ChartPoint getPoint(const int _index);
    };
}
