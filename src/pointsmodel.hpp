#pragma once

#include <QAbstractListModel>
#include <vector>
#include <QPointF>

using std::vector;

namespace Charts
{
    struct ChartPoint
    {
        ChartPoint(float distance, float altitude, float velocity = -1, bool valid = true, bool intersects = false)
            : distance(distance)
            , altitude(altitude)
            , velocity(velocity)
            , valid(valid)
            , intersects(intersects)
        {}

        ChartPoint(QPointF coords, float velocity = -1, bool valid = true, bool intersects = false)
            : distance(coords.x())
            , altitude(coords.y())
            , velocity(velocity)
            , valid(valid)
            , intersects(intersects)
        {}

        size_t index = -1;
        float distance = 0;
        float altitude = 0;
        float velocity = -1;
        bool valid = true;
        bool intersects = false;
    };

    class PointsModel : public QAbstractListModel
    {
        Q_OBJECT
        vector<ChartPoint> m_points;

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

            void setPath(const vector<ChartPoint> &_points);
            void updatePoint(const int _index, const ChartPoint &_point);
            void removePath();

            Q_INVOKABLE
            void changePointAltitude(const int _index, const float _altitude);
            ChartPoint getPoint(const int _index);

            signals:
                void pointChanged(const int _index);
    };
}
