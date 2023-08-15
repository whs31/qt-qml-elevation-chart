import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

RowLayout {
    spacing: -10

    component TT : ToolTip { id: control;
        required property string txt
        required property color __col

        visible: parent.hovered || parent.pressed
        font {
            pixelSize: 14
            family: mainfont
            bold: true
        }
        contentItem: Text {
            text: txt
            font: control.font
            color: impl.palette.background
        }
        background: Rectangle {
            color: col
            radius: 3
        }
        delay: 100
    }

    component Notification : ToolButton {
        required property color col
        required property string details
        required property bool shown

        Layout.preferredWidth: 50
        Layout.preferredHeight: 50

        flat: true
        visible: opacity > 0
        enabled: shown
        opacity: shown ? 1 : 0
        Behavior on opacity { NumberAnimation { } }
        icon {
            width: 35
            height: 35
            source: "qrc:/elevation-chart/icons/warning.svg"
            color: col
        }

        TT { __col: col; txt: details }
    }

    Notification { col: impl.palette.error; details: "Для маршрута отсутствуют необходимые карты профиля высот!"; shown: impl.missingTiles}
    Notification { col: impl.palette.error; details: "Маршрут пересекает рельеф в одной из точек!"; shown: true}
    Notification { col: impl.palette.warn; details: "Маршрут не соответствует ЛТХ борта!"; shown: !impl.matchingMetrics }
}