import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import ElevationChartWidget 3.0
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

Item {
    Material.theme: Material.Dark
    Material.accent: impl.palette.info
    Material.primary: impl.palette.accent
    Material.foreground: impl.palette.foreground
    Material.background: impl.palette.background

    FontLoader { id: font_Main; source: "qrc:/elevation-chart/fonts/Overpass.ttf"; }
    FontLoader { id: font_Mono; source: "qrc:/elevation-chart/fonts/UbuntuMono.ttf"; }

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

    ElevationChartImpl
    {
        id: impl
        anchors.fill: parent
        palette {
            background: Catpuccin.mocha.base.hex
            foreground: Catpuccin.mocha.subtext1.hex
            overlay: Catpuccin.mocha.surface0.hex
            overlay2: Catpuccin.mocha.surface1.hex
            accent: Catpuccin.mocha.teal.hex
            warn: Catpuccin.mocha.peach.hex
            error: Catpuccin.mocha.red.hex
            info: Catpuccin.mocha.lavender.hex
        }

        Component.onCompleted: ElevationChartCXXAPI.setSource(impl)

        property int currentBar: -1
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
            }
        }

        Pane {
            id: panelMetrics

            property bool shown: impl.currentBar === 0
            visible: width > 0
            enabled: visible

            width: shown ? implicitWidth : 0
            height: shown ? implicitHeight : 0

            Behavior on width { NumberAnimation { duration: 150; easing.type: Easing.InOutQuad; } }
            Behavior on height { NumberAnimation { duration: 150; easing.type: Easing.InOutQuad; } }
            clip: true

            anchors {
                top: panelTools.bottom
                left: panelTools.left
            }

            Material.background: impl.palette.overlay2
            Material.elevation: 100

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

        Pane {
            id: panelEnvelope

            property bool shown: impl.currentBar === 1
            visible: width > 0
            enabled: visible

            width: shown ? implicitWidth : 0
            height: shown ? implicitHeight : 0

            Behavior on width { NumberAnimation { duration: 150; easing.type: Easing.InOutQuad; } }
            Behavior on height { NumberAnimation { duration: 150; easing.type: Easing.InOutQuad; } }
            clip: true

            anchors {
                top: panelTools.bottom
                left: panelTools.left
                leftMargin: 70
            }

            Material.background: impl.palette.overlay2
            Material.elevation: 100

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
    }
}
