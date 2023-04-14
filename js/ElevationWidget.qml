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
	property bool b_ShowIndexes: true;
	property vector4d vec_Offsets: Qt.vector4d(30, 0, 7, 15); // left top right bottom : x y z w

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

		boundsBehavior: Flickable.StopAtBounds;
		interactive: true;
		flickableDirection: Flickable.HorizontalAndVerticalFlick;

		visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.Fine;
		enabled: visible;

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
			fl_LineWidth: 3;
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

		GLSolidpolygon { id: c_ImplCoridor;
			objectName: "qml_gl_coridor_polygon"; //! required!
			anchors.fill: parent;
			fillColor: c_ImplRoot.s_WarnColor;
			opacity: 0.2;
			visible: true;
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

	GLAxis { id: c_ImplXAxis;
		objectName: "qml_gl_x_axis"; //! required!
		anchors.fill: parent;
		color: c_ImplRoot.s_ForegroundColor;
		visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.Fine;
		offsets: vec_Offsets;
		opacity: 0.7;
//		fontfamily: c_ImplRoot.s_FontFamily;
	}

	Rectangle {
		anchors.top: parent.bottom;
		anchors.topMargin: -vec_Offsets.w;
		anchors.right: parent.right;
		color: c_ImplRoot.s_ForegroundColor;
		visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.Fine;
		height: vec_Offsets.w;
		width: 100;
		opacity: c_ImplXAxis.opacity;

		Text {
			anchors.centerIn: parent;
			horizontalAlignment: Text.AlignHCenter;
			verticalAlignment: Text.AlignVCenter;
			font.family: s_FontFamily;
			color: s_BackgroundColor;
			font.pixelSize: vec_Offsets.w;
			font.bold: true;
			text: "РАССТОЯНИЕ";
		}
	}

	Rectangle {
		anchors.top: parent.top;
		anchors.left: parent.left;
		anchors.leftMargin: vec_Offsets.x;
		color: c_ImplRoot.s_ForegroundColor;
		visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.Fine;
		height: vec_Offsets.w;
		width: 65;
		opacity: c_ImplXAxis.opacity;

		Text {
			anchors.centerIn: parent;
			horizontalAlignment: Text.AlignHCenter;
			verticalAlignment: Text.AlignVCenter;
			font.family: s_FontFamily;
			font.bold: true;
			color: s_BackgroundColor;
			font.pixelSize: vec_Offsets.w;
			text: "ВЫСОТА";
		}
	}

	GLAxis { id: c_ImplYAxis;
		objectName: "qml_gl_y_axis"; //! required!
		anchors.fill: parent;
		color: c_ImplRoot.s_ForegroundColor;
		visible: false;
	}

	Text { id: c_ImplLabelElevationsMissing;
		anchors.fill: parent;
		color: s_ErrorColor;
		text: "ОТСУТСТВУЮТ ИСХОДНЫЕ ДАННЫЕ ПО ВЫСОТЕ";
		font.bold: true;
		font.pixelSize: width / 30;
		font.family: s_FontFamily;
		horizontalAlignment: Text.AlignHCenter;
		verticalAlignment: Text.AlignVCenter;
		visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.ElevationsMissing;
	}
	Text { id: c_ImplLabelPathMissing;
		anchors.fill: parent;
		color: Qt.lighter(s_BackgroundColor, 1.5);
		text: "НЕ ЗАДАН МАРШРУТ";
		font.bold: true;
		font.pixelSize: width / 30;
		font.family: s_FontFamily;
		horizontalAlignment: Text.AlignHCenter;
		verticalAlignment: Text.AlignVCenter;
		visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.PathMissing;
	}

}
