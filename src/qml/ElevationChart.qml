import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import ElevationChartWidget 3.0
import "qrc:/elevation-chart/catpuccin.js" as Catpuccin

Item {
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
            onEditingFinished: if(action) action.trigger()
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
            accent: Catpuccin.mocha.teal.hex
            warn: Catpuccin.mocha.peach.hex
            error: Catpuccin.mocha.red.hex
            info: Catpuccin.mocha.lavender.hex
        }

        Component.onCompleted: ElevationChartCXXAPI.setSource(impl)

        Pane {
            id: panelTools
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
                    checkable: true
                    radius: 4
                    icon {
                        source: "qrc:/elevation-chart/icons/inspect-graph.svg"
                    }
                }
            }
        }

        Pane {
            id: panelMetrics

            property bool shown: buttonMetrics.checked
            visible: width > 0
            enabled: visible

            width: shown ? implicitWidth : 0
            height: shown ? implicitHeight : 0

            Behavior on width { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad; } }
            Behavior on height { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad; } }
            clip: true

            anchors {
                top: panelTools.bottom
                left: panelTools.left
            }

            Material.background: impl.palette.overlay
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
                    icon.source: "qrc:/elevation-chart/icons/match.svg"
                    Material.background: impl.palette.overlay
                }
            }
        }
    }
}
