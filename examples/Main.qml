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

    color: "#dce0e8";

    ElevationChartModule.ElevationChart {
        anchors.fill: parent;

        backgroundColor: "#181926";
        foregroundColor: "#b8c0e0";
        plottingColor: "#5b6078";
    }
}
