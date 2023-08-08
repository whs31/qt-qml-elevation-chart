import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import "qrc:/elevation-chart"
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

ApplicationWindow {
    id: root

    title: "Elevation Chart Demo"
    minimumWidth: 720
    minimumHeight: 360
    width: 1280
    height: 800
    visible: true
    color: Catpuccin.latte.red.hex

    ElevationChart {
        anchors.fill: parent
    }
}
