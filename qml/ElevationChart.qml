import QtQuick 2.15
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
        VelocityPoint ??? // z j,jchfkcz)
        impl.setPolyline([])
    }
}
