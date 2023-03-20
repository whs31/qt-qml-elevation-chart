import QtQuick 2.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Rectangle { id: root;
	color: "#1b1f23";

	property real random_dist: 0;
	function getRandomVec1()
	{
		return Math.random(new Date().getSeconds());
	}

	Timer { id: timer1; running: true; repeat: true; interval: 750; onTriggered: {
			var p = []; p = scrollingSeries1.points; p.splice(1, 1);  p.push(5500 * getRandomVec1());
			scrollingSeries1.points = p; } }
	Timer { id: timer2; running: true; repeat: true; interval: 50; onTriggered: {
			var p = []; p = scrollingSeries2.points; p.splice(1, 1);  p.push(1000 * getRandomVec1());
			scrollingSeries2.points = p; } }

	ElevationWidget { id: elevationWidget;
		anchors.left: parent.left;
		anchors.right: parent.right;
		anchors.bottom: parent.bottom;
		anchors.top: scrollingSeries1.bottom;

		sFontFamily: "Ubuntu Mono";
	}

	ScrollSeries { id: scrollingSeries1;
		height: 200;
		anchors.left: elevationWidget.left;
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
