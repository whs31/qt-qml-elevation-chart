import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Text {
    required property bool shown
    required property color col
    required property string txt

    text: txt
    font {
        family: mainfont
        pixelSize: 86
        weight: Font.ExtraBold
        capitalization: Font.AllUppercase
    }

    color: col
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    visible: opacity > 0
    enabled: shown
    opacity: shown ? 1 : 0
    Behavior on opacity { NumberAnimation { } }
}