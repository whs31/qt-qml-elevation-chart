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
//		anchors.leftMargin: -10;
//		anchors.rightMargin: -10;
//		anchors.topMargin: -100;
//		anchors.bottomMargin: -100;
		//hoverEnabled: true;
		//acceptedButtons: Qt.RightButton;
//		onPressed: b_Active = true;
//		onReleased: b_Active = false;
		property var pt_GlobalPosition: mapToItem(c_ImplGlobalMouseArea, mouseX, mouseY);
		onContainsMouseChanged: console.error("contains", containsMouse);
		onPressedChanged: console.error("pressed", pressed);
		onPt_GlobalPositionChanged: console.info(pt_GlobalPosition);

		onPressed: c_ImplView.interactive = false;
		onReleased: c_ImplView.interactive = true;
		onCanceled: c_ImplView.interactive = true;
		onPositionChanged: {
//			let x_coord = (pt_GlobalPosition.x < width / 2) ? width / 2 :
			PointModel.changePointAltitude(index, pt_GlobalPosition.y);
//			if(pressed) {
//				let global_pos = mapToItem(c_ImplView, mouseX, mouseY);
//				console.log(global_pos.y);
//				PointModel.changePointAltitude(index, global_pos.y);
//			}
		}
	}
}
