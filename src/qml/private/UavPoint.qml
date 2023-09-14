import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

RoundButton {
    property bool shown: zoom <= 1.1
    opacity: shown ? 1.0 : 0.0
    visible: opacity > 0
    enabled: visible
    width: 23
    height: 23
    padding: 3
    leftInset: 0
    rightInset: 0
    topInset: 0
    bottomInset: 0
    x: impl.uavVisualPosition.x - width / 2
    y: impl.uavVisualPosition.y - height / 2
    transform: Scale {
        xScale: 1 / zoom
        //yScale: 1 / zoom
        origin.x: width / 2
        origin.y: height / 2
    }
    icon {
        source: "qrc:/elevationChart/icons/gps.svg"
        color: impl.palette.background
    }
    layer {
        enabled: true
        smooth: true
        samples: 8
    }
    rotation: 90 - impl.uavVisualAngle

    Material.background: impl.palette.uav
    palette.button: impl.palette.uav
    Behavior on x { NumberAnimation {} }
    Behavior on y { NumberAnimation {} }
    Behavior on opacity { NumberAnimation { easing.type: Easing.InOutQuad }}
}