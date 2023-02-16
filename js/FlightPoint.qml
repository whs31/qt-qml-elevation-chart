import QtQuick 2.15
import QtQuick.Shapes 1.15

Rectangle
{
	property real distance: m_distance;
	property real elevation: m_elevation;

	width: pointSize;
	height: width;
	color: invalid ? base.warningColor : flightPathColor;
	radius: width / 2;
	x: m_x;
	y: m_y;
	border.width: 3;
	border.color: invalid ? Qt.lighter(base.warningColor, 1.2) : Qt.lighter(flightPathColor, 1.2);
	property real delta: y - globalMouseArea.mouseY;
	property bool invalid: m_invalid;

	Timer { id: updateTimer; repeat: true; running: false; interval: 16;
			onTriggered: backend.changeFlightPointAltitude(index, delta); }
	MouseArea { id: pointMouseArea;
		//propagateComposedEvents: true;
		hoverEnabled: false;
		anchors.fill: parent;
		anchors.margins: -1;
		onDoubleClicked: { updateTimer.start(); ui.opacity = 0.9; }
		onClicked: { updateTimer.stop(); ui.opacity = 0; }
	}
	Connections
	{
		target: globalMouseArea;
		function onStopDrag()
		{
			updateTimer.stop();
			ui.opacity = 0;
		}
	}

	Rectangle { id: indexUI;
		visible: base.showIndex;
		width: 31; height: 17; radius: height / 2; opacity: 1; color: invalid ? base.warningColor : flightPathColor;
		anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;
		anchors.horizontalCenterOffset: m_x / base.width > 0.5 ? -width / 2 - 7 : width / 2 + 7;
		Text {
			id: label1; color: base.color; width: 31; height: 15;
			text: "<b>" + Number(index) + "</b>";
			anchors.centerIn: parent; horizontalAlignment: Text.AlignHCenter;
			verticalAlignment: Text.AlignVCenter; font.pointSize: 10;
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
				strokeColor: legend.color; fillColor: legend.color; startX: 0; startY: arrowUp.height;
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
				strokeColor: legend.color; fillColor: legend.color; startX: 0; startY: 0;
				PathLine { x: arrowDown.width / 2; y: arrowDown.height }
				PathLine { x: arrowDown.width; y: 0 }
				PathLine { x: 0; y: 0 }
			}
		}
		Rectangle { id: tooltip;
			width: 160; height: 35; radius: height / 2; color: Qt.lighter(base.color, 2);
			anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;
			anchors.horizontalCenterOffset: m_x / base.width > 0.5 ? -width / 2 - 10 : width / 2 + 10;
			Text {
				id: label2; color: legend.color; width: 140; height: 16;
				text: "Высота: <b>" + Number(elevation).toFixed(1) + " м </b>";
				anchors.horizontalCenter: parent.horizontalCenter;
				anchors.top: parent.top;
				horizontalAlignment: Text.AlignHCenter;
			}
			Text {
				id: label3; color: legend.color; width: 140; height: 16;
				text: "Расстояние: <b>" + Number(distance / 1000).toFixed(1) + " км </b>";
				anchors.horizontalCenter: parent.horizontalCenter;
				anchors.top: label2.bottom;
				horizontalAlignment: Text.AlignHCenter;
			}
		}
	}
}
