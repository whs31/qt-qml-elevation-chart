import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Popup {
    //clip: true

    y: parent.height

    palette.window: impl.palette.overlay2
    Material.background: impl.palette.overlay2
    Material.theme: lightmode ? Material.Light : Material.Dark
    Material.accent: impl.palette.accent
    Material.primary: impl.palette.accent
    Material.foreground: impl.palette.foreground
    Material.elevation: 200
}