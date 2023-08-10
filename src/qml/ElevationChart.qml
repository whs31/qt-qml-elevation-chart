import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtPositioning 5.15
import ElevationChartWidget 3.0

import "private" as Private
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

Item {
    Material.theme: impl.light_theme ? Material.Light : Material.Dark
    Material.accent: impl.palette.accent
    Material.primary: impl.palette.accent
    Material.foreground: impl.palette.foreground
    Material.background: impl.palette.background

    FontLoader { id: font_Main; source: "qrc:/elevation-chart/fonts/Overpass.ttf"; }
    FontLoader { id: font_Mono; source: "qrc:/elevation-chart/fonts/UbuntuMono.ttf"; }

    layer {
        enabled: true
        samples: 8
        smooth: true
    }

    property string mainfont: font_Main.name
    property string monofont: font_Mono.name

    component DecimalInput: RowLayout {
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
            onEditingFinished: if(action) action.trigger(this)
        }
    }

    component CollapsiblePanel : Pane {
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

        Material.background: impl.palette.overlay2
        Material.elevation: 200
    }

    ElevationChartImpl
    {
        id: impl
        anchors.fill: parent
        palette {
            background: light_theme ? Catpuccin.latte.base.hex : Catpuccin.mocha.base.hex
            foreground: light_theme ? Catpuccin.latte.subtext1.hex : Catpuccin.mocha.subtext1.hex
            overlay: light_theme ? Catpuccin.latte.surface1.hex : Catpuccin.mocha.surface0.hex
            overlay2: light_theme ? Catpuccin.latte.surface2.hex : Catpuccin.mocha.surface1.hex
            accent: light_theme ? Catpuccin.latte.green.hex : Catpuccin.mocha.green.hex
            warn: light_theme ? Catpuccin.latte.peach.hex : Catpuccin.mocha.peach.hex
            error: light_theme ? Catpuccin.latte.red.hex : Catpuccin.mocha.red.hex
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
        route: Types.route(QtPositioning.path([QtPositioning.coordinate(60, 30, 100),
                                               QtPositioning.coordinate(60.02, 30, 50),
                                               QtPositioning.coordinate(60.04, 30, 120),
                                               QtPositioning.coordinate(60.06, 30, 70),
                                               QtPositioning.coordinate(60.08, 30, 69),
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

    CollapsiblePanel {
        id: panelMetrics
        index: 0
        offset: 0

        ColumnLayout {
            DecimalInput { description: "Скороподъемность"; defaultText: "10.0"; foregroundColor: impl.palette.foreground; }
            DecimalInput { description: "Скорость спуска"; defaultText: "10.0"; foregroundColor: impl.palette.foreground; }
            DecimalInput { description: "Горизонтальная скорость"; defaultText: "10.0"; foregroundColor: impl.palette.foreground; }
            RoundButton {
                Layout.fillWidth: true
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
            }
        }
    }

    CollapsiblePanel {
        id: panelEnvelope
        index: 1
        offset: 70

        ColumnLayout {
            DecimalInput { description: "Высота огибания"; defaultText: "100.0"; foregroundColor: impl.palette.foreground; }
            DecimalInput { description: "Ширина коридора"; defaultText: "10.0"; foregroundColor: impl.palette.foreground; }
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

    CollapsiblePanel {
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
