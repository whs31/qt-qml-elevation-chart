#pragma once

#include <vector>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtQuick/QQuickItem>
#include <LPVL/ChartBase>

using std::vector;

namespace Widgets
{
    class ElevationChart : public LPVL::ChartBase
    {
        Q_OBJECT

        struct ChartPoint
        {
            double latitude;
            double longitude;
            float altitude;
            float velocity;
        };

        public:
            ElevationChart(QQuickItem* parent = nullptr);

            /*! @brief setPolyline
             *  @details
             *  @param route
             *  @param velocities
             *  @throws std::invalid_argument, если route.size() != velocities.size()
             *          and velocities.size() != 0. !*/
            Q_INVOKABLE void setPolyline(const QList<QVariant>& route, const QList<float> velocities = {});

                signals:

        protected:
            QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;

        private:
            void requestUpdate();

        private:
            vector<ChartPoint> m_route;
    };
} // Widgets
