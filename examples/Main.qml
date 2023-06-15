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
    color: "#dce0e8";

    ElevationChartModule.ElevationChart { id: chart;
        anchors.fill: parent;

        backgroundColor: "#181926";
        foregroundColor: "#b8c0e0";
        plottingColor: "#5b6078";
    }

    Component.onCompleted:
    {
        show();
        var route = [
            QtPositioning.coordinate(60, 30, 0),
            QtPositioning.coordinate(60.5, 30, 50),
            QtPositioning.coordinate(60, 30.5, 40),
            QtPositioning.coordinate(60.5, 30.5, 100)
        ];

        var vels = [
            60,
            60,
            30,
            30
        ];

        // Важно!
        // Если запустить программу в дебаг моде, то исключение из С++ будет показано в логе.
        chart.set(route, vels);
    }
}
