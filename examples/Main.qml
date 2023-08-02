import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    id: root

    Material.theme: Material.Dark
    //Material.accent: ColorTheme.active.color(ColorTheme.Primary)
    //Material.primary: ColorTheme.active.color(ColorTheme.Accent)
    //Material.foreground: ColorTheme.active.color(ColorTheme.Text)
    //Material.background: ColorTheme.active.color(ColorTheme.BaseShade)

    title: "Elevation Chart Demo"
    minimumWidth: 720
    minimumHeight: 360
    width: 1280
    height: 800
    visible: true
    //color: ColorTheme.active.color(ColorTheme.Dark)
}
