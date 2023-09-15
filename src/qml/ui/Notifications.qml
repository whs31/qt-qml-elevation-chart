import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

ListView {
    add: Transition { NumberAnimation { property: "scale"; from: 0; to: 1; duration: 400; easing.type: Easing.OutCubic; } }
    remove: Transition { NumberAnimation { property: "scale"; from: 1; to: 0; duration: 500; easing.type: Easing.OutCubic; } }

    Layout.preferredHeight: parent.height - 3
    height: parent.height
    interactive: false
    layoutDirection: Qt.RightToLeft
    orientation: Qt.Horizontal
    model: impl.notifications
    width: 1000
    delegate: RoundButton {
        required property bool major
        required property string details

        //anchors.right: parent ? parent.right : undefined
        opacity: 0.8
        height: 40

        icon {
            source: "qrc:/elevationChart/icons/warning.svg"
            color: impl.palette.background
        }

        radius: 5
        palette.button: major ? impl.palette.error : impl.palette.warn
        palette.buttonText: impl.palette.background
        Material.background: major ? impl.palette.error : impl.palette.warn
        Material.foreground: impl.palette.background
        Material.elevation: 200
        font {
            pixelSize: 14
            family: mainfont
            bold: true
        }
        text: details
    }
}
