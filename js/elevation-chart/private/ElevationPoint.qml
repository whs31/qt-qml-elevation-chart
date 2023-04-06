import QtQuick 2.15
import CDeclarativePoint 1.0

Item {
	width: 15;
	height: 15;
	x: c_ImplRoot.vec_Offsets.x + distance - width / 2;
	y: altitude - height / 2;
	CDeclarativePoint {
		anchors.fill: parent;
		radius: 7;
		color: c_ImplRoot.s_RouteColor;
	}
}
