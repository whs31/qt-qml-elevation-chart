import QtQuick 2.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

import CElevationWidgetPolyline 1.0
//import "qrc:/qplotjs" as QPlot

Rectangle { id: root;
	color: "#1b1f23";
	CElevationWidgetPolyline
	{
		anchors.fill: parent;
		anchors.margins: 50;
	}
}
