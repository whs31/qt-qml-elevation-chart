import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Pane {
    property int index: 0
    property int offset: 0

    property bool __shown: impl.currentBar === index
    visible: width > 0
    enabled: visible

    width: __shown ? implicitWidth : 0
    height: __shown ? implicitHeight : 0

    Behavior on width { NumberAnimation { duration: 150; easing.type: Easing.InOutQuad; } }
    Behavior on height { NumberAnimation { duration: 150; easing.type: Easing.InOutQuad; } }
    clip: true

    anchors {
        top: panelTools.bottom
        left: panelTools.left
        leftMargin: offset
    }

    palette.window: impl.palette.overlay2
    Material.background: impl.palette.overlay2
    Material.elevation: 200
}