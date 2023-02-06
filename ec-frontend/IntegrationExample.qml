import QtQuick 2.15
import QtPositioning 5.15

Rectangle {
	id: root;
	color: "#3e4c4f";
	ElevationChart
	{
		id: elevationChart;
		anchors.fill: parent;
		anchors.margins: 20;
		anchors.topMargin: 50;

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
		chartColor: "#43a1ca";
		chartVerticalStretch: 1.5;
	}
}
