import QtQuick 2.15
import QtQuick.Controls 2.15
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
	property vector4d vec_Offsets: Qt.vector4d(30, 0, 30, 15); // left top right bottom : x y z w

	property bool b_ZoomToPoint: true;
    property real pointsScale: (groupItemScale.xScale < 1) ? 1 : groupItemScale.xScale;

	// private:
	color: s_BackgroundColor;
	layer.enabled: true;
	layer.samples: 8;

	Connections {
		target: ElevationWidgetBackend;
		function onRouteChanged() {
			c_ImplGlobalMouseArea.zoom = 1;
		}
	}

	ScrollBar { id: horizontalScrollBar;
		property bool isUpdatable: true;
		visible: true;
		hoverEnabled: true;
		active: hovered || pressed;
		orientation: Qt.Horizontal;
		size: groupItemScale.scaledItemsWidth / c_ImplView.width - 0.0001;
		policy: ScrollBar.AlwaysOn;
		height: 10;
		anchors.left: parent.left;
		anchors.right: parent.right;
        anchors.bottom: parent.bottom;
		position: groupItemScale.scaleWindowPosition / c_ImplView.width;
        onPositionChanged: {
			if(isUpdatable) {
				groupItemScale.origin.x = (c_ImplView.width / (1 - size)) * position;
            }
        }
    }

	Flickable { id: c_ImplView;
		anchors.fill: parent;
		anchors.leftMargin: vec_Offsets.x;
		anchors.bottomMargin: vec_Offsets.w;
		anchors.rightMargin: vec_Offsets.z;
		anchors.topMargin: vec_Offsets.y;

		boundsBehavior: Flickable.StopAtBounds;
		boundsMovement: Flickable.StopAtBounds;
		interactive: false;
		flickableDirection: Flickable.HorizontalAndVerticalFlick;

		visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.Fine;
		enabled: visible;
		clip: true;

		MouseArea { id: c_ImplGlobalMouseArea;
			readonly property real fl_MaxZoom: 15000;
			readonly property real fl_ZoomStep: 1.5;
			property real zoom: 1;
			anchors.fill: parent;
			hoverEnabled: true;
		}

		Item {
			id: groupItem;
			width: parent.width;
			height: parent.height;
            transform: Scale {
				id: groupItemScale;
				property int scaledItemsWidth: c_ImplView.width / xScale;
				property int originX:magicMouseArea.mouseX;
                property int scaleWindowPosition: 0; //позиция окна после зума относительно основного изображения
				origin.x: c_ImplView.width / 2;
                onXScaleChanged: {
					if(xScale <= 1)
						xScale = 1;

					let originPrecent = origin.x / c_ImplView.width;
					let delta = scaledItemsWidth * originPrecent;
					scaleWindowPosition = origin.x - delta;

                }

                function changeXScale(newXOrigin, newXScale){
                    let oldScale = groupItemScale.xScale;
                    let oldOrigin = groupItemScale.origin.x;
                    let oldOriginPrecent = oldOrigin / c_ImplView.width
                    let newOriginPrecent = newXOrigin / c_ImplView.width
                    let deltaPrecent = newOriginPrecent - oldOriginPrecent
                    let deltaVelue = deltaPrecent * scaledItemsWidth;
                    groupItemScale.origin.x += deltaVelue;
                    groupItemScale.xScale = newXScale;
                }

            }
            MouseArea{
				id: magicMouseArea;
				anchors.fill: parent;
				hoverEnabled: true;
				propagateComposedEvents: true;
                onWheel: {
					horizontalScrollBar.isUpdatable = false;
                    if(wheel.angleDelta.y > 0 ) {
						groupItemScale.changeXScale(magicMouseArea.mouseX , groupItemScale.xScale * 1.1);
                    }
					else if(wheel.angleDelta.y < 0 ) {
						groupItemScale.changeXScale(magicMouseArea.mouseX , groupItemScale.xScale / 1.1);
                    }
					horizontalScrollBar.isUpdatable = true;
                }
            }

            GLPolygon { id: c_ImplProfile;
                objectName: "qml_gl_profile_polygon"; //! required!
                anchors.fill: parent;
                fillColor: c_ImplRoot.s_ProfileColor;
                visible: true;
            }

            GLPolyline { id: c_ImplMetricsPath;
                objectName: "qml_gl_metrics_polyline"; //! required!
                anchors.fill: parent;
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
                anchors.fill: parent;
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
                anchors.fill: parent;
                lineColor: c_ImplRoot.s_RouteColor;
                visible: true;
            }

            Repeater {
                clip: false;
                model: PointModel;
                anchors.fill: parent;
				delegate: Private.ElevationPoint { scaleValue : pointsScale }
            }
        }
	}
    GLAxis { id: c_ImplXAxis;
        objectName: "qml_gl_x_axis"; //! required!
        anchors.fill: parent;
        color: c_ImplRoot.s_ForegroundColor;
        visible: ElevationWidgetBackend.state === ElevationWidgetBackend.WidgetState.Fine;
        offsets: vec_Offsets;
        opacity: 0.7;
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
		anchors.right: parent.right;
		anchors.rightMargin: vec_Offsets.z;
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
			font.bold: true;
			color: s_BackgroundColor;
			font.pixelSize: vec_Offsets.w;
			text: "АБС. ВЫСОТА";
		}
	}

	Rectangle {
		anchors.top: parent.top;
		anchors.left: parent.left;
		anchors.leftMargin: vec_Offsets.x;
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
			font.bold: true;
			color: s_BackgroundColor;
			font.pixelSize: vec_Offsets.w;
			text: "ОТН. ВЫСОТА";
		}
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
