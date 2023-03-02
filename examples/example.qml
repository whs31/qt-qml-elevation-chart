import QtQuick 2.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Rectangle { id: root;
	color: "#1b1f23";
	property real coord1: 60.1;
	property real coord2: 30.15;

	property real random_dist: 0;
	function getRandomVec1()
	{
		return Math.random(new Date().getSeconds());
	}

	function getRandomGeoCoordinate()
	{
		var num = (Math.random(new Date().getSeconds()));
		return QtPositioning.coordinate(60 + num / 25, 30 + num / 25, 50 + (num - 0.5) * 50);
	}

	Timer { id: timer1; running: true; repeat: true; interval: 750; onTriggered: {
			var p = []; p = scrollingSeries1.points; p.splice(1, 1);  p.push(5500 * getRandomVec1());
			scrollingSeries1.points = p; } }
	Timer { id: timer2; running: true; repeat: true; interval: 50; onTriggered: {
			var p = []; p = scrollingSeries2.points; p.splice(1, 1);  p.push(1000 * getRandomVec1());
			scrollingSeries2.points = p; } }

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
			onMoved: { coord1 = value; elevationChart.updatePath(); }
		}
		Slider { id: slider2;
			anchors.top: slider1.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			anchors.margins: 5;
			from: 30;
			to: 30.15;
			value: 30.15;
			onMoved: { coord2 = value; elevationChart.updatePath(); }
		}
		CheckBox { id: checkbox1;
			anchors.top: slider2.bottom;
			anchors.left: parent.left;
			anchors.right: parent.right;
			anchors.margins: 5;
			checked: true;
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
		height: 200;

		path: QtPositioning.path([
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 QtPositioning.coordinate(coord1, coord2, 100)
		]);
		horizontalVelocity: parseInt(textField1.text);
		rateOfClimb: parseInt(textField2.text);
		rateOfDescend: parseInt(textField3.text);

		logging: false;
		showIndex: checkbox1.checked;
		showFPSCounter: true;

		backgroundColor: "#263238";
		chartColor: "#607D8B";
		flightPathColor: "#9CCC65";
		successColor: "#9CCC65";
		warningColor: "#FFAB40";
		errorColor: "#FF7043";

		pointSize: 15;
	}

	ScrollSeries { id: scrollingSeries1;
		height: 200;
		anchors.left: elevationChart.left;
		width: 300;
		anchors.top: parent.top;

		pointsCount: 30;

		backgroundColor: "#263238";
		graphColor: "#FFAB40";
		legendColor: "#fafbfc";

		Component.onCompleted: {
			var p = [];
			for(let i = 0; i < 1000; i++)
			{
				p.push(5500 * getRandomVec1());
			}
			scrollingSeries1.points = p;
		}
	}

	ScrollSeries { id: scrollingSeries2;
		anchors.bottom: scrollingSeries1.bottom;
		anchors.left: scrollingSeries1.right;
		width: 300;
		anchors.top: parent.top;

		pointsCount: 150;

		backgroundColor: "#263238";
		graphColor: "#9CCC65";
		legendColor: "#fafbfc";

		Component.onCompleted: {
			var p = [];
			for(let i = 0; i < 1000; i++)
			{
				p.push(1000 * getRandomVec1());
			}
			scrollingSeries2.points = p;
		}
	}
}
