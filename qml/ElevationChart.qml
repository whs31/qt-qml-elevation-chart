import QtQuick 2.15
import QtPositioning 5.15
import Widgets.impl 1.0

Item {
    property alias backgroundColor: impl.backgroundColor;
    property alias foregroundColor: impl.foregroundColor;
    property alias plottingColor: impl.plottingColor;

    ElevationChartBackend { id: impl;
        anchors.fill: parent;
    }

    Component.onCompleted:
    {
        var route = [
            QtPositioning.coordinate(60, 30, 0),
            QtPositioning.coordinate(61, 30, 50),
            QtPositioning.coordinate(60, 31, 40),
            QtPositioning.coordinate(61, 31, 100)
        ];

        var vels = [
            60,
            60,
            30,
            16
        ];
        impl.setPolyline(route, vels);
    }
}
