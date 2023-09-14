import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtPositioning 5.15
import ElevationChartWidget 3.0
import "qrc:/elevationChart"
import "qrc:/elevationChart/catpuccin.js" as Catpuccin

ApplicationWindow {
    id: root

    title: "Elevation Chart Demo"
    minimumWidth: 720
    minimumHeight: 360
    width: 1900
    height: 800
    visible: true
    color: Catpuccin.latte.red.hex

    property real testing_route: testingSlider.value
    ElevationChart {
        id: elevationChart
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
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

        uavPosition: QtPositioning.coordinate(60, 30.01, 100)
        Timer {
            property int x: 0
            property var list: [QtPositioning.coordinate(60, 30.01, 100),
                QtPositioning.coordinate(testing_route, 30.01, 50),
                QtPositioning.coordinate(60.04, 30.01, 120),
                QtPositioning.coordinate(60.06, 30.01, 70),
                QtPositioning.coordinate(60.08, 30.01, 69),
                QtPositioning.coordinate(60.1, 30.02, 56),
                QtPositioning.coordinate(60.1, 30.04, 110),
                QtPositioning.coordinate(60.1, 30.06, 45),
                QtPositioning.coordinate(60.1, 30.08, 55),
                QtPositioning.coordinate(60.1, 30.1, 70)]
            running: true
            repeat: true
            interval: 1000
            onTriggered: {
                x++
                if(x > 9)
                    x = 0
                parent.uavPosition = list[x]
            }
        }
        //Behavior on uavPosition { CoordinateAnimation { } }
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
