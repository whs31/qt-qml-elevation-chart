import QtQuick 2.15
import QtPositioning 5.15

Rectangle {
	id: root;
	color: "#000000";
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
					QtPositioning.coordinate(60.1, 30.15, 35),
					QtPositioning.coordinate(60.15, 30.15, 23),
					QtPositioning.coordinate(60.1, 30.2, 0),
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
}
