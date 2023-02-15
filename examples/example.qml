import QtQuick 2.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Rectangle { id: root;
	color: "#1b1f23";
	property real coord1: 60.1;
	property real coord2: 30.15;

	Rectangle { id: exampleToolPanel;
		anchors.right: parent.right;
		anchors.top: parent.top;
		anchors.bottom: parent.bottom;
		width: 250;
		color: "#24292e";

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
		Text { id: label1;
			anchors.top: checkbox1.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			text: "Горизонтальная скорость";
			color: "#FAFAFA";
		}
		TextField { id: textField1;
			validator: IntValidator {bottom: 1; top: 10000;}
			text: "75";
			anchors.top: label1.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
		}
		Text { id: label2;
			anchors.top: textField1.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			text: "Скороподъемность";
			color: "#FAFAFA";
		}
		TextField { id: textField2;
			validator: IntValidator {bottom: 1; top: 10000;}
			text: "1";
			anchors.top: label2.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
		}
		Text { id: label3;
			anchors.top: textField2.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			text: "Скорость спуска";
			color: "#FAFAFA";
		}
		TextField { id: textField3;
			validator: IntValidator {bottom: 1; top: 10000;}
			text: "1";
			anchors.top: label3.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
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
		horizontalVelocity: parseInt(textField1.text);
		rateOfClimb: parseInt(textField2.text);
		rateOfDescend: parseInt(textField3.text);

		logging: false;
		showIndex: checkbox1.checked;

		backgroundColor: "#2f363d";
		chartColor: "#586069";
		flightPathColor: "#f9c513";
		successColor: "#22863a";
		warningColor: "#e36209";
		errorColor: "#cb2431";

		pointSize: 15;
	}
}
