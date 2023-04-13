import QtQuick 2.15
import GLShapes 1.0
import ElevationWidgetModule 1.0

Item { id: c_ImplPoint;
	width: 15;
	height: 15;
	x: distance - width / 2;
	y: altitude - height / 2;
	property bool b_Active: false;
	z: 100;

	GLPoint {
		anchors.fill: parent;
		radius: 7;
		color: c_ImplRoot.s_RouteColor;

		Rectangle {
			visible: c_ImplRoot.b_ShowIndexes;
			width: 15;
			height: 13;
			color: c_ImplRoot.s_RouteColor;
			radius: 3;
			anchors.bottom: parent.top;
			anchors.bottomMargin: 3;
			anchors.horizontalCenter: parent.horizontalCenter;

			Text {
				text: Number(index + 1);
				color: c_ImplRoot.s_BackgroundColor;
				font.pixelSize: 10;
				font.bold: true;
				anchors.centerIn: parent;
				horizontalAlignment: Text.AlignHCenter;
				verticalAlignment: Text.AlignVCenter;
				font.family: c_ImplRoot.s_FontFamily;
			}
		}
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
