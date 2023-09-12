import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

RowLayout {
    property string description: "None"
    property string defaultText: "0.00"
    property color foregroundColor: "red"
    property Action action: null
    opacity: enabled ? 1 : 0.4

    Text {
        Layout.fillWidth: true
        text: description + ":"
        color: foregroundColor
        font {
            family: mainfont
            weight: Font.DemiBold
            pixelSize: 14
        }
    }

    TextField {
        validator: RegExpValidator { regExp: /^[0-9]*(\.[0-9]{0,2})?$/ }
        selectByMouse: true
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        text: defaultText
        font {
            family: mainfont
            weight: Font.Bold
            pixelSize: 14
        }
        horizontalAlignment: Text.AlignRight
        onTextChanged: if(action) action.trigger(this)
    }
}