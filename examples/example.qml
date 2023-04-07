import QtQuick 2.15
import "qrc:/shared" as Shared

Rectangle { id: root;
	color: "#1b1f23";

	ElevationWidget {
		anchors.fill: parent;
	}

	Shared.GUIFreezeIndicator {
		anchors.left: parent.left;
		anchors.top: parent.top;
		anchors.margins: 50;
	}
}
