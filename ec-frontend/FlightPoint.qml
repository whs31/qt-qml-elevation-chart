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
	property int direction: 0;
	width: m_width;
	height: width;
	color: m_color;
	radius: width / 2;
	x: m_x;
	y: m_y;
	border.width: 3;
	border.color: Qt.lighter(m_color, 1.2);

	Timer { id: timer; running: direction !== 0; interval: 25; repeat: true; onTriggered:
	{
		backend.changeFlightPointAltitude(index, wheelHandler.shiftPressed ? direction * 10 : direction);
	} }

	MouseArea
	{
		propagateComposedEvents: true;
		hoverEnabled: true;
		anchors.fill: parent;
		anchors.margins: -20;
		anchors.bottomMargin: parent.height / 2;
		anchors.topMargin: -300;
		onEntered: {
			parent.color = uiColor;
			arrowUp.opacity = 1;
			tooltip.opacity = 0.9;
		}
		onExited: {
			parent.color = m_color;
			arrowUp.opacity = 0;
			arrowDown.opacity = 0;
			direction = 0;
			tooltip.opacity = 0;
		}
		onPressed: direction = 1;
		onReleased: direction = 0;
		onContainsMouseChanged: if(!containsMouse) { direction = 0; parent.color = m_color; arrowUp.opacity = 0; arrowDown.opacity = 0; tooltip.opacity = 0; }
		onPressedChanged: if(!pressed) { direction = 0; }
	}
	MouseArea
	{
		propagateComposedEvents: true;
		hoverEnabled: true;
		anchors.fill: parent;
		anchors.margins: -20;
		anchors.bottomMargin: -300;
		anchors.topMargin: parent.height / 2;
		onEntered: {
			parent.color = uiColor;
			arrowDown.opacity = 1;
			tooltip.opacity = 0.9;
		}
		onExited: {
			parent.color = m_color;
			arrowUp.opacity = 0;
			arrowDown.opacity = 0;
			direction = 0;
			tooltip.opacity = 0;
		}
		onPressed: direction = -1;
		onReleased: direction = 0;
		onContainsMouseChanged: if(!containsMouse) { direction = 0; parent.color = m_color; arrowUp.opacity = 0; arrowDown.opacity = 0; tooltip.opacity = 0; }
		onPressedChanged: if(!pressed) { direction = 0; }
	}

	Shape
	{
		id: arrowUp;
		opacity: 0;
		width: 15;
		height: 10;
		anchors.bottom: parent.top;
		anchors.bottomMargin: 5;
		anchors.horizontalCenter: parent.horizontalCenter;
		smooth: true;
		antialiasing: true;
		ShapePath {
			strokeWidth: 0;
			capStyle: ShapePath.RoundCap;
			joinStyle: ShapePath.RoundJoin;
			strokeColor: successColor;
			fillColor: successColor;
			startX: 0; startY: arrowUp.height;
			PathLine { x: arrowUp.width / 2; y: 0 }
			PathLine { x: arrowUp.width; y: arrowUp.height }
			PathLine { x: 0; y: arrowUp.height }
		}
		Behavior on opacity { NumberAnimation { duration: 200; } }
	}
	Shape
	{
		id: arrowDown;
		opacity: 0;
		width: 15;
		height: 10;
		anchors.top: parent.bottom;
		anchors.topMargin: 5;
		anchors.horizontalCenter: parent.horizontalCenter;
		smooth: true;
		antialiasing: true;
		ShapePath {
			strokeWidth: 0;
			capStyle: ShapePath.RoundCap;
			joinStyle: ShapePath.RoundJoin;
			strokeColor: errorColor;
			fillColor: errorColor;
			startX: 0; startY: 0;
			PathLine { x: arrowDown.width / 2; y: arrowDown.height }
			PathLine { x: arrowDown.width; y: 0 }
			PathLine { x: 0; y: 0 }
		}
		Behavior on opacity { NumberAnimation { duration: 200; } }
	}
	Rectangle
	{
		id: tooltip;
		opacity: 0;
		Behavior on opacity { NumberAnimation { duration: 200; } }
		width: 150;
		height: 50;
		radius: 5;
		color: uiBackgroundColor;
		anchors.left: parent.right;
		anchors.leftMargin: 4;
		anchors.verticalCenter: parent.verticalCenter;
		Text {
			id: label1;
			text: "Индекс точки: <b>" + Number(index) + "</b>";
			color: uiColor;
			width: 140;
			height: 16;
			anchors.horizontalCenter: parent.horizontalCenter;
			anchors.top: parent.top;
		}
		Text {
			id: label2;
			text: "Высота: <b>" + Number(elevation).toFixed(1) + " м </b>";
			color: uiColor;
			width: 140;
			height: 16;
			anchors.horizontalCenter: parent.horizontalCenter;
			anchors.top: label1.bottom;
		}
		Text {
			id: label3;
			text: "Расстояние: <b>" + Number(distance / 1000).toFixed(1) + " км </b>";
			color: uiColor;
			width: 140;
			height: 16;
			anchors.horizontalCenter: parent.horizontalCenter;
			anchors.top: label2.bottom;
		}
	}
}
