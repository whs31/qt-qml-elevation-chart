import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

RoundButton {
    required property int index
    required property real altitude
    required property real distance
    required property bool valid
    required property bool intersects

    Material.background: impl.palette.accent

    width: 25
    height: 25

    x: (distance / impl.bounds.x * impl.width) - width / 2
    y: (impl.height - (altitude / (impl.bounds.y * impl.bounds.stretch) * impl.height)) - height / 2

    onPressYChanged: impl.model.move(index, -pressY)

    component TT : ToolTip { id: control;
        property string txt: "Tooltip";

        visible: parent.hovered || parent.pressed
        font {
            pixelSize: 13
            family: mainfont
        }
        contentItem: Text {
            text: txt
            font: control.font
            color: impl.palette.background
        }
        background: Rectangle {
            color: impl.palette.foreground
            radius: 3
        }
        delay: 0
    }

    TT { txt: "<b>Высота точки: </b>" + Number(altitude).toFixed(1) + " м<br><b>Удаленность точки: </b>" + Number(distance).toFixed(0) + " м" }

    Rectangle {
        property bool shown: ec.showIndexes

        opacity: shown ? 0.9 : 0
        enabled: shown
        Behavior on opacity { NumberAnimation { duration: 250 } }
        width: 16;
        height: 14;
        color: impl.palette.accent
        radius: 3

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.top
            bottomMargin: -3
        }

        Text {
            text: Number(index + 1)
            color: impl.palette.background
            font {
                pixelSize: 10
                family: mainfont
                bold: true
            }
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
    }
}