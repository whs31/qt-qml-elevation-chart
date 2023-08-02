import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import "qrc:/elevation-chart"
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

ApplicationWindow {
    id: root

    Material.theme: Material.Light
    Material.accent: Catpuccin.latte.blue.hex
    Material.primary: Catpuccin.latte.mauve.hex
    Material.foreground: Catpuccin.latte.text.hex
    Material.background: Catpuccin.latte.base.hex

    title: "Elevation Chart Demo"
    minimumWidth: 720
    minimumHeight: 360
    width: 1280
    height: 800
    visible: true
    color: Catpuccin.latte.base.hex

    ElevationChart {
        anchors.centerIn: parent
    }
}
