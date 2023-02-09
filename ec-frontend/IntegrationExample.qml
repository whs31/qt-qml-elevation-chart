import QtQuick 2.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Rectangle {
	id: root;
	color: "#000000";
	property real coord1: 60.1;
	property real coord2: 30.15;

	ElevationChart
	{
		id: elevationChart;
		anchors.fill: parent;
		anchors.margins: 20;
		anchors.topMargin: 50;

		logging: false;
		path: QtPositioning.path([
					QtPositioning.coordinate(60, 30, 60),
					QtPositioning.coordinate(60.1, 30.1, 20),
					QtPositioning.coordinate(60.15, 30.1, 40),
					QtPositioning.coordinate(coord1, coord2, 35),
					QtPositioning.coordinate(coord1+0.05, coord2, 0),
					QtPositioning.coordinate(60.05, 30.05, 11)
				],
				7);
		color: "#11151C";
		chartColor: "#6C5980";
		flightPathColor: "#E6B450";
		successColor: "#7FD962";
		errorColor: "#D95757";
		chartVerticalStretch: 1.5;
	}

	Slider
	{
		id: slider1;
		anchors.top: parent.top;
		anchors.left: parent.left;
		anchors.bottom: elevationChart.top;
		anchors.right: parent.horizontalCenter;
		anchors.margins: 5;
		from: 60;
		to: 60.1;
		value: 60.05;
		onMoved: { coord1 = value; }
	}
	Slider
	{
		id: slider2;
		anchors.top: parent.top;
		anchors.left: parent.horizontalCenter;
		anchors.bottom: elevationChart.top;
		anchors.right: parent.right;
		anchors.margins: 5;
		from: 30;
		to: 30.15;
		value: 30.15;
		onMoved: { coord2 = value; }
	}
}
