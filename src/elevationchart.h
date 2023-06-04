#pragma once

#include <QtCore/QObject>
#include <QtQuick/QQuickItem>
#include <LPVL/ChartBase>

namespace Widgets
{
    class ElevationChart : public LPVL::ChartBase
    {
        Q_OBJECT

        public:
            ElevationChart(QQuickItem* parent = nullptr);

            signals:

    };
} // Widgets
