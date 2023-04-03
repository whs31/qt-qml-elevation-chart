import QtQuick 2.15
import QtGraphicalEffects 1.15

import CDeclarativePolyline 1.0
import CDeclarativePolygon 1.0

Rectangle { id: c_ImplRoot;
	property color s_BackgroundColor: Qt.darker("#2E3440", 1.2);

	property color s_ProfileColor: "#4c566a";
	property color s_RouteColor: "#ebcb8b";

	color: s_BackgroundColor;
	layer.enabled: true;
	layer.samples: 8;

	CDeclarativePolygon { id: c_ImplProfile;
		objectName: "qml_gl_profile_polygon"; //! required!
		anchors.fill: parent;
		anchors.leftMargin: 30;
		anchors.bottomMargin: 15;
		anchors.rightMargin: 7;
		fillColor: c_ImplRoot.s_ProfileColor;
		visible: true;
	}

	CDeclarativePolyline { id: c_ImplBasePath;
		objectName: "qml_gl_path_polyline"; //! required!
		anchors.fill: c_ImplProfile;
		lineColor: c_ImplRoot.s_RouteColor;
		visible: true;
	}

}
