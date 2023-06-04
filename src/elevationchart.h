#pragma once

#include <vector>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtQuick/QQuickItem>
#include <LPVL/ChartBase>
#include "velocitypoint.h"

namespace Widgets
{
    class ElevationChart : public LPVL::ChartBase
    {
        Q_OBJECT

        public:
            ElevationChart(QQuickItem* parent = nullptr);

            Q_INVOKABLE void setPolyline(const QList<Widgets::VelocityPoint>& route);

            signals:

        protected:
            QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;

    };
} // Widgets
