import QtQuick 2.15
import ElevationChartWidget 3.0
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

Item {
    ElevationChartImpl
    {
        anchors.fill: parent
        palette.background: Catpuccin.mocha.base.hex
    }
}
