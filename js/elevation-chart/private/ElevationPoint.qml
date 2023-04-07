import QtQuick 2.15
import CDeclarativePoint 1.0
import PointModel 1.0

Item {
	width: 15;
	height: 15;
	x: distance - width / 2;
	y: altitude - height / 2;
	property bool b_Active: false;

	CDeclarativePoint {
		anchors.fill: parent;
		radius: 7;
		color: c_ImplRoot.s_RouteColor;
	}
	MouseArea { id: pointMouseArea;
		anchors.fill: parent;
		anchors.leftMargin: -10;
		anchors.rightMargin: -10;
		anchors.topMargin: -100;
		anchors.bottomMargin: -100;
		hoverEnabled: true;
		acceptedButtons: Qt.RightButton;
		onPressed: b_Active = true;
		onReleased: b_Active = false;

		onPositionChanged: {
			if(b_Active) {
				let global_pos = mapToItem(c_ImplView, mouseX, mouseY);
				console.log(global_pos.y);
				PointModel.changePointAltitude(index, global_pos.y);
			}
		}
	}
}
