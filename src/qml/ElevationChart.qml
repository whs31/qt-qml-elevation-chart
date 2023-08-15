import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtPositioning 5.15
import ElevationChartWidget 3.0

import "private" as Private
import "ui" as UI
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

Rectangle {
    id: ec
    property alias route: impl.route

    Material.theme: impl.light_theme ? Material.Light : Material.Dark
    Material.accent: impl.palette.accent
    Material.primary: impl.palette.accent
    Material.foreground: impl.palette.foreground
    Material.background: impl.palette.background
    color: impl.palette.background

    FontLoader { id: font_Main; source: "qrc:/elevation-chart/fonts/Overpass.ttf"; }
    FontLoader { id: font_Mono; source: "qrc:/elevation-chart/fonts/UbuntuMono.ttf"; }

    layer {
        enabled: true
        samples: 8
        smooth: true
    }

    property string mainfont: font_Main.name
    property string monofont: font_Mono.name

    property bool showIndexes: true

    ElevationChartImpl
    {
        id: impl
        anchors {
            fill: parent
            leftMargin: 10
            rightMargin: 10
            bottomMargin: 10
        }
        palette {
            background: light_theme ? Catpuccin.latte.base.hex : Catpuccin.mocha.base.hex
            foreground: light_theme ? Catpuccin.latte.subtext1.hex : Catpuccin.mocha.subtext1.hex
            overlay: light_theme ? Catpuccin.latte.surface2.hex : Catpuccin.mocha.surface0.hex
            overlay2: light_theme ? Catpuccin.latte.overlay0.hex : Catpuccin.mocha.surface1.hex
            accent: light_theme ? Catpuccin.latte.text.hex : Catpuccin.mocha.text.hex
            warn: light_theme ? Catpuccin.latte.yellow.hex : Catpuccin.mocha.yellow.hex
            error: light_theme ? Catpuccin.latte.maroon.hex : Catpuccin.mocha.maroon.hex
            info: light_theme ? Catpuccin.latte.lavender.hex : Catpuccin.mocha.lavender.hex

            Behavior on background { ColorAnimation { easing.type: Easing.InOutQuad } }
            Behavior on foreground { ColorAnimation { easing.type: Easing.InOutQuad } }
            Behavior on overlay { ColorAnimation { easing.type: Easing.InOutQuad } }
            Behavior on overlay2 { ColorAnimation { easing.type: Easing.InOutQuad } }
            Behavior on accent { ColorAnimation { easing.type: Easing.InOutQuad } }
            Behavior on warn { ColorAnimation { easing.type: Easing.InOutQuad } }
            Behavior on error { ColorAnimation { easing.type: Easing.InOutQuad } }
            Behavior on info { ColorAnimation { easing.type: Easing.InOutQuad } }
        }
        route: Types.route(QtPositioning.path([QtPositioning.coordinate(60, 30.01, 100),
                                               QtPositioning.coordinate(60.02, 30.01, 50),
                                               QtPositioning.coordinate(60.04, 30.01, 120),
                                               QtPositioning.coordinate(60.06, 30.01, 70),
                                               QtPositioning.coordinate(60.08, 30.01, 69),
                                               QtPositioning.coordinate(60.1, 30.02, 56),
                                               QtPositioning.coordinate(60.1, 30.04, 110),
                                               QtPositioning.coordinate(60.1, 30.06, 45),
                                               QtPositioning.coordinate(60.1, 30.08, 55),
                                               QtPositioning.coordinate(60.1, 30.1, 70)]))

        Component.onCompleted: ElevationChartCXXAPI.setSource(impl)

        property int currentBar: -1
        property bool light_theme: false

        Repeater {
            anchors.fill: parent
            model: impl.model
            delegate: Private.ElevationChartPoint { }
        }
    }

    UI.LargeWarning { shown: impl.missingTiles; txt: "Отсутствуют профили высот"; col: impl.palette.overlay; anchors.centerIn: parent }
    UI.LargeWarning { shown: !impl.route.valid(); txt: "Не задан путь"; col: impl.palette.overlay; anchors.centerIn: parent }

    UI.Notifications {
        id: notifications
        anchors {
            top: parent.top
            right: parent.right
            margins: 10
        }
    }

    Pane {
        id: panelTools
        opacity: 0.75
        Material.background: impl.palette.overlay
        Material.elevation: 100

        anchors {
            top: parent.top
            left: parent.left
            margins: 10
        }

        RowLayout {
            RoundButton {
                id: buttonMetrics
                radius: 4
                icon {
                    source: "qrc:/elevation-chart/icons/inspect-graph.svg"
                    color: impl.currentBar === 0 ? impl.palette.background : impl.palette.foreground
                }

                Material.background: impl.currentBar === 0 ? impl.palette.warn : impl.palette.overlay2

                onPressed: {
                    if(impl.currentBar !== 0)
                        impl.currentBar = 0
                    else
                        impl.currentBar = -1
                }
            }

            RoundButton {
                id: buttonEnvelope
                radius: 4
                icon {
                    source: "qrc:/elevation-chart/icons/envelope.svg"
                    color: impl.currentBar === 1 ? impl.palette.background : impl.palette.foreground
                }

                Material.background: impl.currentBar === 1 ? impl.palette.info : impl.palette.overlay2

                onPressed: {
                    if(impl.currentBar !== 1)
                        impl.currentBar = 1
                    else
                        impl.currentBar = -1
                }
            }

            RoundButton {
                id: buttonSettings
                radius: 4
                icon {
                    source: "qrc:/elevation-chart/icons/settings.svg"
                    color: impl.currentBar === 2 ? impl.palette.background : impl.palette.foreground
                }

                Material.background: impl.currentBar === 2 ? impl.palette.accent : impl.palette.overlay2

                onPressed: {
                    if(impl.currentBar !== 2)
                        impl.currentBar = 2
                    else
                        impl.currentBar = -1
                }
            }
        }
    }

    UI.CollapsiblePanel {
        id: panelMetrics
        index: 0
        offset: 0

        ColumnLayout {
            UI.DecimalInput { description: "Скороподъемность"; defaultText: "1.0"; foregroundColor: impl.palette.foreground; action: Action { onTriggered: impl.metrics.rateOfClimb = parseFloat(source.text) } }
            UI.DecimalInput { description: "Скорость спуска"; defaultText: "1.0"; foregroundColor: impl.palette.foreground; action: Action { onTriggered: impl.metrics.rateOfDescend = parseFloat(source.text) } }
            UI.DecimalInput { description: "Горизонтальная скорость"; defaultText: "75.0"; foregroundColor: impl.palette.foreground; action: Action { onTriggered: impl.metrics.fallbackVelocity = parseFloat(source.text) } }
            RoundButton {
                Layout.fillWidth: true
                enabled: !impl.matchingMetrics
                text: "Применить коррекцию по ЛТХ"
                radius: 4
                font {
                    family: mainfont
                    weight: Font.DemiBold
                    pixelSize: 14
                }
                icon {
                    source: "qrc:/elevation-chart/icons/match.svg"
                }

                Material.background: impl.palette.overlay2

                onPressed: impl.applyMetricsCorrection()
            }
        }
    }

    UI.CollapsiblePanel {
        id: panelEnvelope
        index: 1
        offset: 70

        ColumnLayout {
            UI.DecimalInput { description: "Высота огибания"; defaultText: "100.0"; foregroundColor: impl.palette.foreground; }
            UI.DecimalInput { description: "Ширина коридора"; defaultText: "10.0"; foregroundColor: impl.palette.foreground; }
            RoundButton {
                Layout.fillWidth: true
                text: "Вычислить огибающую"
                radius: 4
                font {
                    family: mainfont
                    weight: Font.DemiBold
                    pixelSize: 14
                }
                icon {
                    source: "qrc:/elevation-chart/icons/resize.svg"
                }

                Material.background: impl.palette.overlay2
            }

            RoundButton {
                Layout.fillWidth: true
                text: "Применить огибающую"
                radius: 4
                font {
                    family: mainfont
                    weight: Font.DemiBold
                    pixelSize: 14
                }
                icon {
                    source: "qrc:/elevation-chart/icons/polyline.svg"
                }

                Material.background: impl.palette.overlay2
            }
        }
    }

    UI.CollapsiblePanel {
        id: panelSettings
        index: 2
        offset: 125

        ColumnLayout {
            Switch {
                icon {
                    source: "qqrc:/elevation-chart/icons/label.svg"
                }
                font {
                    family: mainfont
                    weight: Font.DemiBold
                    pixelSize: 14
                }
                text: "Отображать индексы точек"
                checked: true

                Material.background: impl.palette.overlay2
                onCheckedChanged: showIndexes = checked
            }

            Button {
                icon {
                    source: impl.light_theme ? "qrc:/elevation-chart/icons/light.svg" : "qrc:/elevation-chart/icons/dark.svg"
                }
                font {
                    family: mainfont
                    weight: Font.DemiBold
                    pixelSize: 14
                }
                flat: true
                Layout.fillWidth: true
                text: "Переключить тему"
                onPressed: impl.light_theme = !impl.light_theme
            }
        }
    }
}
