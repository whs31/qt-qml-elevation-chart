import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtPositioning 5.15
import ElevationChartWidget 3.0
import "qrc:/elevation-chart"
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

ApplicationWindow {
    id: root

    title: "Elevation Chart Demo"
    minimumWidth: 720
    minimumHeight: 360
    width: 1900
    height: 400
    visible: true
    color: Catpuccin.latte.red.hex

    property real testing_route: testingSlider.value
    ElevationChart {
        id: elevationChart
        anchors.fill: parent
        route: testingButton.checked ? Types.route(QtPositioning.path([QtPositioning.coordinate(60, 30.01, 100),
            QtPositioning.coordinate(testing_route, 30.01, 50),
            QtPositioning.coordinate(60.04, 30.01, 120),
            QtPositioning.coordinate(60.06, 30.01, 70),
            QtPositioning.coordinate(60.08, 30.01, 69),
            QtPositioning.coordinate(60.1, 30.02, 56),
            QtPositioning.coordinate(60.1, 30.04, 110),
            QtPositioning.coordinate(60.1, 30.06, 45),
            QtPositioning.coordinate(60.1, 30.08, 55),
            QtPositioning.coordinate(60.1, 30.1, 70)]))
            : Types.route(QtPositioning.path())
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.left

        RoundButton {
            id: testingButton
            text: checked ? "RESET PATH" : "SET PATH"
            checkable: true
            checked: true
        }

        Slider {
            id: testingSlider
            value: 60
            from: 60
            to: 62
        }
    }
}
