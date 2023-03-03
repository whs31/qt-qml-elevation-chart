import QtQuick 2.15
import QtQuick.Controls 2.15
import ElevationWidgetImpl 1.0
import "private" as Private;

Rectangle { id: base;
	focus: true;
	clip: true;
	color: Impl.colors[0];

	Component.onCompleted:
	{
		Impl.resize(width, height, property_handler.zoomW, 1);
	}
	onWidthChanged: Impl.resize(width, height, property_handler.zoomW, 1);
	onHeightChanged: Impl.resize(width, height, property_handler.zoomW, 1);

	Item { id: property_handler;
		property real zoomW: 1;
		Behavior on zoomW { NumberAnimation { duration: 250; } }
		onZoomWChanged: {
			if(zoomW > wheelHandler.maxZoom)
				zoomW = wheelHandler.maxZoom;
			if(zoomW < 1)
			{
				zoomW = 1;
			}
			Impl.resize(base.width, base.height, zoomW, 1);
		}
	}

	property real widthScaled: width * z_w;
	property real z_w: property_handler.zoomW;

	Connections {
		target: Impl;
		function onRequestAll() {
			requestAll();
		}
		function onRequestIntersects() {
			//intersects.requestPaint();
		}

		function onRequestPath() {
			//graph.requestPaint();
			legendImpl.refresh();
		}
	}

	function requestAll()
	{
		//graph.requestPaint();
		legendImpl.refresh();
		profileImpl.refresh();
	}

	ListModel { id: pathModel; }
	ScrollBar { id: scrollbar;
		anchors.top: view.top;
		anchors.left: view.left;
		anchors.right: view.right;
		implicitHeight: 10;
		contentItem: Rectangle {
			radius: scrollbar.height / 2;
			color: scrollbar.pressed ? Qt.lighter(Impl.colors[2], 1.5) : Impl.colors[2];
		}
	}
	Flickable { id: view;
		anchors.fill: parent;
		contentWidth: base.width * property_handler.zoomW;
		contentHeight: base.height;
		flickableDirection: Flickable.HorizontalAndVerticalFlick;
		interactive: false;
		boundsMovement: Flickable.StopAtBounds;
		clip: true;
		pixelAligned: true;
		ScrollBar.horizontal: scrollbar;
		onMovementEnded: requestAll();

		Private.ElevationWidgetProfile { id: profileImpl; }
	}


	Private.ElevationWidgetLegend { id: legendImpl; anchors.fill: parent; }

	Keys.onPressed: { if (event.key === Qt.Key_Shift) { wheelHandler.shiftPressed = true } }
	Keys.onReleased: { if (event.key === Qt.Key_Shift) { wheelHandler.shiftPressed = false } }
	WheelHandler { id: wheelHandler;
		property real maxZoom: 1000;
		readonly property real zoom_sensivity: 3;
		property bool shiftPressed: false;

		onWheel: (event) =>
		{
			if(!shiftPressed) {
				if(event.angleDelta.y > 0 && property_handler.zoomW <= maxZoom)
					property_handler.zoomW += (1 / zoom_sensivity) * property_handler.zoomW;
				else if(event.angleDelta.y < 0 && property_handler.zoomW >= 1)
					property_handler.zoomW -= (1 / zoom_sensivity) * property_handler.zoomW;
			} else {
				if(event.angleDelta.y > 0)
					view.flick(1000, 0);
				else if(event.angleDelta.y < 0)
					view.flick(-1000, 0);
			}
		}
	}
	MouseArea { id: globalMouseArea;
		acceptedButtons: Qt.RightButton;
		anchors.fill: parent;
		hoverEnabled: true;
		//onPositionChanged: mouseCross.requestPaint();
	}
}
