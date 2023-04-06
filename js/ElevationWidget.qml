import QtQuick 2.15

import CDeclarativePolyline 1.0
import CDeclarativePolygon 1.0

Rectangle { id: c_ImplRoot;
	property color s_BackgroundColor: Qt.darker("#2E3440", 1.2);
	property color s_ForegroundColor: "#d8dee9";
	property color s_ProfileColor: "#4c566a";
	property color s_RouteColor: "#a3be8c";
	property color s_WarnColor: "#ebcb8b";
	property color s_ErrorColor: "#bf616a";
	property color s_InfoColor: "#81a1c1";

	property string s_FontFamily: "Ubuntu Mono";

	// private:
	color: s_BackgroundColor;
	layer.enabled: true;
	layer.samples: 8;

	Flickable
	{
		anchors.fill: parent;
		interactive: true;
		flickableDirection: Flickable.HorizontalAndVerticalFlick;

		CDeclarativePolygon { id: c_ImplProfile;
			objectName: "qml_gl_profile_polygon"; //! required!
			anchors.fill: parent;
			anchors.leftMargin: 30;
			anchors.bottomMargin: 15;
			anchors.rightMargin: 7;
			fillColor: c_ImplRoot.s_ProfileColor;
			visible: true;
		}

		CDeclarativePolyline { id: c_ImplMetricsPath;
			objectName: "qml_gl_metrics_polyline"; //! required!
			anchors.fill: c_ImplProfile;
			lineColor: c_ImplRoot.s_WarnColor;
			visible: true;
		}

		CDeclarativePolyline { id: c_ImplEnvelopePath;
			objectName: "qml_gl_envelope_polyline"; //! required!
			anchors.fill: c_ImplProfile;
			lineColor: c_ImplRoot.s_InfoColor;
			visible: true;
			opacity: 0.5;
			dashed: true;
			SequentialAnimation {
				PropertyAnimation {
					target: c_ImplEnvelopePath;
					property: "opacity";
					to: 0.5;
					duration: 500;
					easing.type: Easing.InOutQuad;
				}
				PropertyAnimation {
					target: c_ImplEnvelopePath;
					property: "opacity";
					to: 1.0;
					duration: 500;
					easing.type: Easing.InOutQuad;
				}

				loops: Animation.Infinite;
				running: true;
				Component.onCompleted: start();
			}
		}

		CDeclarativePolyline { id: c_ImplBasePath;
			objectName: "qml_gl_path_polyline"; //! required!
			anchors.fill: c_ImplProfile;
			lineColor: c_ImplRoot.s_RouteColor;
			visible: true;
		}
	}
}
