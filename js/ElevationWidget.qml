import QtQuick 2.15
import ElevationWidgetImpl 1.0

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

	WheelHandler { id: wheelHandler;
		property real maxZoom: 1000;
		readonly property real zoom_sensivity: 3;
		property bool shiftPressed: false;

		onWheel: (event) =>
		{
			if(!shiftPressed) {
				if(event.angleDelta.y > 0 && backend.zoomX <= maxZoom)
					property_handler.zoomW += (1 / zoom_sensivity) * property_handler.zoomW;
				else if(event.angleDelta.y < 0 && property_handler.zoomX >= 1)
					property_handler.zoomW -= (1 / zoom_sensivity) * property_handler.zoomW;
			} else {
				//if(event.angleDelta.y > 0)
					//view.flick(500, 0);
				//else if(event.angleDelta.y < 0)
					//view.flick(-500, 0);
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
