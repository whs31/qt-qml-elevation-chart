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
		var now = new Date();
		var seed = now.getSeconds();
		var num = (Math.random(seed));
		return num;
	}

	function getRandomGeoCoordinate()
	{
		var now = new Date();
		var seed = now.getSeconds();
		var num = (Math.random(seed));
		var a = QtPositioning.coordinate(60 + num / 25, 30 + num / 25, num * 100);
		return a;
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
		anchors.top: parent.verticalCenter;

		path: QtPositioning.path([
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate(),
									 getRandomGeoCoordinate()
		]);
		horizontalVelocity: parseInt(textField1.text);
		rateOfClimb: parseInt(textField2.text);
		rateOfDescend: parseInt(textField3.text);

		logging: false;
		showIndex: checkbox1.checked;

		backgroundColor: "#24292e";
		chartColor: "#586069";
		flightPathColor: "#ffab70";
		successColor: "#34d058";
		warningColor: "#f66a0a";
		errorColor: "#ea4a5a";

		pointSize: 15;
	}

	ScrollSeries { id: scrollingSeries1;
		anchors.bottom: elevationChart.top;
		anchors.left: elevationChart.left;
		anchors.right: parent.horizontalCenter;
		anchors.top: parent.top;
		anchors.bottomMargin: 5;

		pointsCount: 30;

		backgroundColor: "#24292e";
		graphColor: "#ffab70";
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
		anchors.bottom: elevationChart.top;
		anchors.left: parent.horizontalCenter;
		anchors.right: exampleToolPanel.left;
		anchors.top: parent.top;
		anchors.bottomMargin: 5;

		pointsCount: 150;

		backgroundColor: "#24292e";
		graphColor: "#34d058";
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
