import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15

import "qrc:/elevation-chart" as ElevationChartModule

ApplicationWindow {
    title: "Elevation Chart Testing Window";
    width: 1280;
    height: 720;
    visible: true;
    Component.onCompleted: show();

    color: "#181926";

    ElevationChartModule.ElevationChart
    {
        anchors.fill: parent;
    }
}
