import QtQuick 2.15
import ElevationChartWidget 3.0
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

Item {
    ElevationChartImpl
    {
        anchors.fill: parent
        palette {
            background: Catpuccin.mocha.base.hex
            foreground: Catpuccin.mocha.subtext1.hex
            overlay: Catpuccin.mocha.surface2.hex
            accent: Catpuccin.mocha.teal.hex
            warn: Catpuccin.mocha.peach.hex
            error: Catpuccin.mocha.red.hex
            info: Catpuccin.mocha.lavender.hex
        }
    }
}
