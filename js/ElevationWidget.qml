import QtQuick 2.15

import GLShapes 1.0
import ElevationWidgetModule 1.0
import "elevation-chart/private" as Private

Rectangle { id: c_ImplRoot;
	property color s_BackgroundColor: Qt.darker("#2E3440", 1.2);
	property color s_ForegroundColor: "#d8dee9";
	property color s_ProfileColor: "#4c566a";
	property color s_RouteColor: "#a3be8c";
	property color s_WarnColor: "#ebcb8b";
	property color s_ErrorColor: "#bf616a";
	property color s_InfoColor: "#81a1c1";

	property string s_FontFamily: "Ubuntu Mono";

	property vector4d vec_Offsets: Qt.vector4d(30, 0, 15, 7); // left top right bottom : x y z w

	// private:
	color: s_BackgroundColor;
	layer.enabled: true;
	layer.samples: 8;

	Flickable { id: c_ImplView;
		anchors.fill: parent;
		anchors.leftMargin: vec_Offsets.x;
		anchors.bottomMargin: vec_Offsets.w;
		anchors.rightMargin: vec_Offsets.z;
		anchors.topMargin: vec_Offsets.y;

		interactive: false;
		flickableDirection: Flickable.HorizontalAndVerticalFlick;

		MouseArea { id: c_ImplGlobalMouseArea;
			anchors.fill: parent;
			hoverEnabled: true;
			//propagateComposedEvents: true;
		}

		GLPolygon { id: c_ImplProfile;
			objectName: "qml_gl_profile_polygon"; //! required!
			anchors.fill: parent;
			fillColor: c_ImplRoot.s_ProfileColor;
			visible: true;
		}

		GLPolyline { id: c_ImplMetricsPath;
			objectName: "qml_gl_metrics_polyline"; //! required!
			anchors.fill: c_ImplProfile;
			lineColor: c_ImplRoot.s_WarnColor;
			visible: true;
			opacity: 0.5;
			SequentialAnimation {
				PropertyAnimation {
					target: c_ImplMetricsPath;
					property: "opacity";
					to: 0;
					duration: 500;
					easing.type: Easing.InOutQuad;
				}
				PropertyAnimation {
					target: c_ImplMetricsPath;
					property: "opacity";
					to: 0.5;
					duration: 500;
					easing.type: Easing.InOutQuad;
				}

				loops: Animation.Infinite;
				running: true;
				Component.onCompleted: start();
			}
		}

		GLPolyline { id: c_ImplEnvelopePath;
			objectName: "qml_gl_envelope_polyline"; //! required!
			anchors.fill: c_ImplProfile;
			lineColor: c_ImplRoot.s_InfoColor;
			visible: true;
			opacity: 0.5;
			SequentialAnimation {
				PropertyAnimation {
					target: c_ImplEnvelopePath;
					property: "opacity";
					to: 0;
					duration: 500;
					easing.type: Easing.InOutQuad;
				}
				PropertyAnimation {
					target: c_ImplEnvelopePath;
					property: "opacity";
					to: 0.5;
					duration: 500;
					easing.type: Easing.InOutQuad;
				}

				loops: Animation.Infinite;
				running: true;
				Component.onCompleted: start();
			}
		}

		GLMultipolygon { id: c_ImplIntersects;
			objectName: "qml_gl_intersects_polygon"; //! required!
			anchors.fill: parent;
			fillColor: c_ImplRoot.s_ErrorColor;
			visible: true;
		}

		GLPolyline { id: c_ImplBasePath;
			objectName: "qml_gl_path_polyline"; //! required!
			anchors.fill: c_ImplProfile;
			lineColor: c_ImplRoot.s_RouteColor;
			visible: true;
		}

		Repeater
		{
			clip: false;
			model: PointModel;
			anchors.fill: c_ImplProfile;
			delegate: Private.ElevationPoint { }
		}
	}
}
