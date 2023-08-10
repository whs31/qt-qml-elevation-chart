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

    onPressYChanged: {
        impl.model.move(index, -pressY)
    }
}