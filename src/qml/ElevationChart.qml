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
    property alias palette: impl.palette

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
    property real zoom: pinchArea.m_zoom2

    Flickable {
        id: flick
        anchors {
            fill: parent
            leftMargin: 7
            rightMargin: 7
            bottomMargin: 7
        }

        //clip: true
        boundsBehavior: Flickable.StopAtBounds
        boundsMovement: Flickable.StopAtBounds

        Item {
            id: rect
            width: flick.width
            height: flick.height
            transform: Scale {
                id: scaler
                origin.x: pinchArea.m_x2
                origin.y: pinchArea.m_y2
                xScale: pinchArea.m_zoom2
                //yScale: pinchArea.m_zoom2
            }

            ElevationChartImpl {
                id: impl
                z: dragArea.z + 1
                anchors.fill: parent
                visible: opacity > 0
                enabled: visible
                opacity: impl.route.valid() && !impl.missingTiles ? 1 : 0
                Behavior on opacity { NumberAnimation { } }

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

                Component.onCompleted: ElevationChartCXXAPI.setSource(impl)

                property int currentBar: -1
                property bool light_theme: false

                Repeater {
                    anchors.fill: parent
                    model: impl.model
                    delegate: Private.ElevationChartPoint { }
                }
            }

            PinchArea {
                id: pinchArea
                anchors.fill: parent
                property real m_x1: 0
                property real m_y1: 0
                property real m_y2: 0
                property real m_x2: 0
                property real m_zoom1: 1
                property real m_zoom2: 1
                property real m_max: 30
                property real m_min: 1

                onPinchStarted: {
                    m_x1 = scaler.origin.x
                    m_y1 = scaler.origin.y
                    m_x2 = pinch.startCenter.x
                    m_y2 = pinch.startCenter.y
                    rect.x = rect.x + (pinchArea.m_x1 - pinchArea.m_x2) * (1 - pinchArea.m_zoom1)
                    //rect.y = rect.y + (pinchArea.m_y1 - pinchArea.m_y2) * (1 - pinchArea.m_zoom1)
                }

                onPinchUpdated: {
                    m_zoom1 = scaler.xScale
                    var dz = pinch.scale - pinch.previousScale
                    var newZoom = m_zoom1 + dz
                    if(newZoom <= m_max && newZoom >= m_min)
                        m_zoom2 = newZoom
                }

                MouseArea {
                    id: dragArea
                    hoverEnabled: true
                    anchors.fill: parent
                    drag.target: rect
                    drag.filterChildren: true
                    drag.axis: Drag.XAxis
                    drag.smoothed: true

                    onWheel: {
                        pinchArea.m_x1 = scaler.origin.x
                        pinchArea.m_y1 = scaler.origin.y
                        pinchArea.m_zoom1 = scaler.xScale
                        pinchArea.m_x2 = mouseX
                        pinchArea.m_y2 = mouseY

                        var newZoom
                        if(wheel.angleDelta.y > 0)
                            {
                            newZoom = pinchArea.m_zoom1 + 0.3;
                            if(newZoom <= pinchArea.m_max)
                                pinchArea.m_zoom2 = newZoom;
                            else
                                pinchArea.m_zoom2 = pinchArea.m_max;
                        }
                        else
                            {
                            newZoom = pinchArea.m_zoom1 - 0.3
                            if(newZoom >= pinchArea.m_min)
                                pinchArea.m_zoom2 = newZoom
                            else
                                pinchArea.m_zoom2 = pinchArea.m_min
                        }
                        rect.x = rect.x + (pinchArea.m_x1 - pinchArea.m_x2) * (1 - pinchArea.m_zoom1);
                        //rect.y = rect.y + (pinchArea.m_y1 - pinchArea.m_y2) * (1 - pinchArea.m_zoom1);
                    }
                }
            }
        }
    }

    UI.LargeWarning { shown: impl.missingTiles && impl.route.valid(); txt: "Отсутствуют профили высот"; col: impl.palette.overlay; anchors.centerIn: parent }
    UI.LargeWarning { shown: !impl.route.valid(); txt: "Не задан путь"; col: impl.palette.overlay; anchors.centerIn: parent }
    Text {
        property bool shown: zoom > 1.1
        property real value: zoom

        anchors {
            right: parent.right
            bottom: parent.bottom
        }

        text: Number(value).toFixed(2) + "x"
        font {
            family: mainfont
            pixelSize: 48
            weight: Font.ExtraBold
        }

        color: impl.palette.overlay2
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter

        visible: opacity > 0
        enabled: shown
        opacity: shown ? 1 : 0
        Behavior on opacity { NumberAnimation { } }
    }

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
