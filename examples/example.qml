import QtQuick 2.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Rectangle { id: root;
	color: "#000000";
	property real coord1: 60.1;
	property real coord2: 30.15;

	Rectangle { id: exampleToolPanel;
		anchors.right: parent.right;
		anchors.top: parent.top;
		anchors.bottom: parent.bottom;
		width: 150;
		color: "#171B24";

		Slider { id: slider1;
			anchors.top: parent.top;
			anchors.left: parent.left;
			anchors.right: parent.right;
			anchors.margins: 5;
			from: 60;
			to: 60.1;
			value: 60.05;
			onMoved: { coord1 = value; }
		}
		Slider { id: slider2;
			anchors.top: slider1.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			anchors.margins: 5;
			from: 30;
			to: 30.15;
			value: 30.15;
			onMoved: { coord2 = value; }
		}
		CheckBox { id: checkbox1;
			anchors.top: slider2.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			anchors.margins: 5;
			checked: false;
			text: "Показать номера точек";
		}
	}

	ElevationChart { id: elevationChart;
		anchors.left: parent.left;
		anchors.right: exampleToolPanel.left;
		anchors.bottom: parent.bottom;
		anchors.top: parent.verticalCenter;

		path: QtPositioning.path([
					QtPositioning.coordinate(60, 30, 60),
					QtPositioning.coordinate(60.1, 30.1, 20),
					QtPositioning.coordinate(60.15, 30.1, 40),
					QtPositioning.coordinate(coord1, coord2, 35),
					QtPositioning.coordinate(coord1+0.05, coord2, 0),
					QtPositioning.coordinate(60.05, 30.05, 11)
		]);

		logging: false;
		showIndex: checkbox1.checked;

		backgroundColor: "#1F2430";
		chartColor: "#707A8C";
		flightPathColor: "#E6B450";
		successColor: "#87D96C";
		errorColor: "#FF6666";

		pointSize: 15;
	}
}
