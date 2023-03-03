import QtQuick 2.15
import QtQuick.Shapes 1.15
import ElevationWidgetImpl 1.0

Rectangle
{
	property real distance: m_distance;
	property real elevation: m_elevation;

	width: pathImpl.flightPointSize;
	height: width;
	color: invalid ? Impl.colors[4] : Impl.colors[3];
	radius: width / 2;
	x: m_x;
	y: m_y;
	border.width: 3;
	border.color: invalid ? Qt.lighter(Impl.colors[4], 1.2) : Qt.lighter(Impl.colors[3], 1.2);
	property real delta: y - pointMouseArea.globalPos.y;
	property bool invalid: m_invalid;

	Timer { id: updateTimer; repeat: true; running: false; interval: 10;
			onTriggered: Impl.changeFlightPointAltitude(index, delta); }
	MouseArea { id: pointMouseArea;
		anchors.fill: parent;
		property var globalPos: mapToItem(globalMouseArea, mouseX, mouseY);
		hoverEnabled: true;
		onPressedChanged:
		{
			if(pressed)
			{
				updateTimer.start(); ui.opacity = 0.9;
			}
			else {
				updateTimer.stop(); ui.opacity = 0;
			}
		}
	}
	Rectangle { id: indexUI;
		visible: Impl.showIndex;
		width: 31; height: 17; radius: height / 2; opacity: 1; color: invalid ? Impl.colors[4] : Impl.colors[3];
		anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;
		anchors.horizontalCenterOffset: m_x / base.width > 0.5 ? -width / 2 - 7 : width / 2 + 7;
		Text {
			id: label1; color: base.color; width: 31; height: 15;
			text: "<b>" + Number(index) + "</b>";
			anchors.centerIn: parent; horizontalAlignment: Text.AlignHCenter;
			verticalAlignment: Text.AlignVCenter; font.pixelSize: 10;
		}
	}

	Item { id: ui;
		anchors.fill: parent;
		opacity: 0;
		Behavior on opacity { NumberAnimation { duration: 200; } }
		Shape { id: arrowUp;
			width: 15; height: 10; anchors.bottom: parent.top; anchors.bottomMargin: 15; opacity: 0.75;
			anchors.horizontalCenter: parent.horizontalCenter; smooth: true; antialiasing: true;
			ShapePath {
				strokeWidth: 0; capStyle: ShapePath.RoundCap; joinStyle: ShapePath.RoundJoin;
				strokeColor: Impl.colors[1]; fillColor: Impl.colors[1]; startX: 0; startY: arrowUp.height;
				PathLine { x: arrowUp.width / 2; y: 0 }
				PathLine { x: arrowUp.width; y: arrowUp.height }
				PathLine { x: 0; y: arrowUp.height }
			}
		}
		Shape { id: arrowDown;
			width: 15; height: 10; anchors.top: parent.bottom; anchors.topMargin: 15; opacity: 0.75;
			anchors.horizontalCenter: parent.horizontalCenter; smooth: true; antialiasing: true;
			ShapePath {
				strokeWidth: 0; capStyle: ShapePath.RoundCap; joinStyle: ShapePath.RoundJoin;
				strokeColor: Impl.colors[1]; fillColor: Impl.colors[1]; startX: 0; startY: 0;
				PathLine { x: arrowDown.width / 2; y: arrowDown.height }
				PathLine { x: arrowDown.width; y: 0 }
				PathLine { x: 0; y: 0 }
			}
		}
		Rectangle { id: tooltip;
			width: 160; height: 35; radius: height / 2; color: Qt.lighter(Impl.colors[0], 2);
			anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;
			anchors.horizontalCenterOffset: m_x / base.width > 0.5 ? -width / 2 - 10 : width / 2 + 10;
			Text {
				id: label2; color: Impl.colors[1]; width: 140; height: 16;
				text: "Высота: <b>" + Number(elevation).toFixed(1) + " м </b>";
				anchors.horizontalCenter: parent.horizontalCenter;
				anchors.top: parent.top;
				horizontalAlignment: Text.AlignHCenter;
			}
			Text {
				id: label3; color: Impl.colors[1]; width: 140; height: 16;
				text: "Расстояние: <b>" + Number(distance / 1000).toFixed(1) + " км </b>";
				anchors.horizontalCenter: parent.horizontalCenter;
				anchors.top: label2.bottom;
				horizontalAlignment: Text.AlignHCenter;
			}
		}
	}
}
