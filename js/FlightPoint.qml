import QtQuick 2.15
import QtQuick.Shapes 1.15

Rectangle
{
	property color uiColor: m_ui_color;
	property color uiBackgroundColor: m_ui_background_color;
	property color successColor: m_success_color;
	property color errorColor: m_error_color;
	property real distance: m_distance;
	property real elevation: m_elevation;

	width: m_width;
	height: width;
	color: m_color;
	radius: width / 2;
	x: m_x;
	y: m_y;
	border.width: 3;
	border.color: Qt.lighter(m_color, 1.2);
	property real delta: y - globalMouseArea.mouseY;

	Timer { id: updateTimer; repeat: true; running: false; interval: 25;
			onTriggered: backend.changeFlightPointAltitude(index, delta); }
	MouseArea { id: pointMouseArea;
		//propagateComposedEvents: true;
		hoverEnabled: true;
		anchors.fill: parent;
		anchors.margins: -10;
		onDoubleClicked: updateTimer.start();
		onClicked: updateTimer.stop();

		onEntered: {
			parent.color = uiColor;
			ui.opacity = 0.9;
		}
		onExited: {
			parent.color = m_color;
			ui.opacity = 0;
		}
		onContainsMouseChanged: if(!containsMouse) { parent.color = m_color; ui.opacity = 0; }
	}
	Connections
	{
		target: globalMouseArea;
		function onStopDrag()
		{
			updateTimer.stop();
		}
	}

	Item { id: ui;
		anchors.fill: parent;
		opacity: 0;
		Behavior on opacity { NumberAnimation { duration: 200; } }
		Shape { id: arrowUp;
			width: 15; height: 10; anchors.bottom: parent.top; anchors.bottomMargin: 5;
			anchors.horizontalCenter: parent.horizontalCenter; smooth: true; antialiasing: true;
			ShapePath {
				strokeWidth: 0; capStyle: ShapePath.RoundCap; joinStyle: ShapePath.RoundJoin;
				strokeColor: successColor; fillColor: successColor; startX: 0; startY: arrowUp.height;
				PathLine { x: arrowUp.width / 2; y: 0 }
				PathLine { x: arrowUp.width; y: arrowUp.height }
				PathLine { x: 0; y: arrowUp.height }
			}
		}
		Shape { id: arrowDown;
			width: 15; height: 10; anchors.top: parent.bottom; anchors.topMargin: 5;
			anchors.horizontalCenter: parent.horizontalCenter; smooth: true; antialiasing: true;
			ShapePath {
				strokeWidth: 0; capStyle: ShapePath.RoundCap; joinStyle: ShapePath.RoundJoin;
				strokeColor: errorColor; fillColor: errorColor; startX: 0; startY: 0;
				PathLine { x: arrowDown.width / 2; y: arrowDown.height }
				PathLine { x: arrowDown.width; y: 0 }
				PathLine { x: 0; y: 0 }
			}
		}
		Rectangle { id: tooltip;
			width: 150; height: 50; radius: 5; color: uiBackgroundColor;
			anchors.left: parent.right; anchors.leftMargin: 4;
			anchors.verticalCenter: parent.verticalCenter;
			Text {
				id: label1; color: uiColor; width: 140; height: 16;
				text: "Индекс точки: <b>" + Number(index) + "</b>";
				anchors.horizontalCenter: parent.horizontalCenter;
				anchors.top: parent.top;
			}
			Text {
				id: label2; color: uiColor; width: 140; height: 16;
				text: "Высота: <b>" + Number(elevation).toFixed(1) + " м </b>";
				anchors.horizontalCenter: parent.horizontalCenter;
				anchors.top: label1.bottom;
			}
			Text {
				id: label3; color: uiColor; width: 140; height: 16;
				text: "Расстояние: <b>" + Number(distance / 1000).toFixed(1) + " км </b>";
				anchors.horizontalCenter: parent.horizontalCenter;
				anchors.top: label2.bottom;
			}
		}
	}
}
