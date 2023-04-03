import QtQuick 2.15
import QtGraphicalEffects 1.15

import CDeclarativePolyline 1.0

Rectangle { id: c_ImplRoot;
	property color s_BackgroundColor: Qt.darker("#2E3440", 1.2);

	property color s_RouteColor: "#EBCB8B";

	color: s_BackgroundColor;
	layer.enabled: true;
	layer.samples: 8;

	CDeclarativePolyline { id: c_ImplBasePath;
		objectName: "qml_gl_path_polyline"; //! required!
		anchors.fill: parent;
		anchors.margins: 50;
		lineColor: c_ImplRoot.s_RouteColor;
		visible: true;
	}
}
