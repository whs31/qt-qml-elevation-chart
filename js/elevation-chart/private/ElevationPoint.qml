import QtQuick 2.15
import GLShapes 1.0
import ElevationWidgetModule 1.0

Item { id: c_ImplPoint;
	width: 15;
	height: 15;
	x: distance - width / 2;
	y: altitude - height / 2;
	property bool b_Active: false;

	GLPoint {
		anchors.fill: parent;
		radius: 7;
		color: c_ImplRoot.s_RouteColor;
	}
	MouseArea { id: pointMouseArea;
		anchors.fill: parent;
		property var pt_GlobalPosition: mapToItem(c_ImplGlobalMouseArea, mouseX, mouseY);

		onPressed: c_ImplView.interactive = false;
		onReleased: c_ImplView.interactive = true;
		onCanceled: c_ImplView.interactive = true;
		onPositionChanged: PointModel.changePointAltitude(index, pt_GlobalPosition.y);
	}
}
