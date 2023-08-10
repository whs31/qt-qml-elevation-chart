import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

RoundButton {
    required property int index
    required property real altitude
    required property real distance
    required property bool valid
    required property bool intersects

    width: 25
    height: 25

    x: distance - width / 2
    y: altitude - height / 2
    onDistanceChanged: console.error(x, y)
}