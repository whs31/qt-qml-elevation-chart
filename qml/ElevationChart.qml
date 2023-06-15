import QtQuick 2.15
import QtPositioning 5.15
import Widgets.impl 1.0
import LPVL.Charts 1.0

Item {
    function set(route, velocities)
    {
        impl.setPolyline(route, velocities);
    }

    property alias backgroundColor: impl.backgroundColor;
    property alias foregroundColor: impl.foregroundColor;
    property alias plottingColor: impl.plottingColor;

    ElevationChartBackend { id: impl;
        anchors.fill: parent;
    }

    LPVLValueMesh { id: mesh;
        opacity: 0.7;
        anchors.fill: parent;
        antialiasing: true;
        smooth: true;
        rectSize: Qt.size(0.1, 0.1);
    }
}
