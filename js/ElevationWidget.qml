import QtQuick 2.15
import QtQuick.Controls 2.15
import ElevationWidgetImpl 1.0
import QtGraphicalEffects 1.15

import "private" as Private;
import "private/delegates" as Delegates

Rectangle { id: base;
	required property string sFontFamily;
	focus: true;
	clip: true;
	color: Impl.colors[0];
	function requestAll()
	{
		correctPathImpl.refresh();
		pathImpl.refresh();
		legendImpl.refresh();
		profileImpl.refresh();
	}
	Component.onCompleted:
	{
		Impl.resize((base.width - base.offset), height, property_handler.zoomW, 1);
	}
	onWidthChanged: Impl.resize((base.width - base.offset), height, property_handler.zoomW, 1);
	onHeightChanged: Impl.resize((base.width - base.offset), height, property_handler.zoomW, 1);

	Item { id: property_handler;
		property real zoomW: 1;
		Behavior on zoomW { NumberAnimation { duration: 250; } }
		onZoomWChanged: {
			if(zoomW > wheelHandler.maxZoom)
				zoomW = wheelHandler.maxZoom;
			if(zoomW < 1)
				zoomW = 1;
			Impl.resize((base.width - base.offset), base.height, zoomW, 1);
		}
	}

	property real widthScaled: (base.width - base.offset) * z_w;
	property real z_w: property_handler.zoomW;
	property int offset: 30;
	MouseArea { id: globalMouseArea;
		acceptedButtons: Qt.RightButton;
		anchors.fill: parent;
		hoverEnabled: true;
		onPositionChanged: mouseCrossImpl.requestPaint();

		Connections {
			target: Impl;
			function onRequestAll() {
				requestAll();
			}
			function onRequestIntersects() {
				intersectsImpl.refresh();
			}

			function onRequestPath() {
				correctPathImpl.refresh();
				pathImpl.refresh();
				legendImpl.refresh();
			}

			function onRequestBounds() {
                //boundsImpl.refresh();
			}
		}

		ListModel { id: pathModel; }
		ScrollBar { id: scrollbar;
			anchors.top: view.top;
			anchors.left: view.left;
			anchors.right: view.right;
			implicitHeight: 10;
			contentItem: Rectangle {
				radius: scrollbar.height / 2;
				color: scrollbar.pressed ? Qt.lighter(Impl.colors[2], .5) : Impl.colors[2];
			}
		}
		Flickable { id: view;
			anchors.fill: legendImpl;
			anchors.leftMargin: base.offset;
			contentWidth: (base.width - base.offset) * property_handler.zoomW;
			contentHeight: base.height;
			flickableDirection: Flickable.HorizontalAndVerticalFlick;
			interactive: false;
			boundsMovement: Flickable.StopAtBounds;
			clip: false;
			pixelAligned: true;
			ScrollBar.horizontal: scrollbar;
			onMovementEnded: requestAll();

			Private.ElevationWidgetProfile { id: profileImpl; visible: Impl.valid && Impl.fileIntegrity; }
			Private.ElevationWidgetCorrectPath { id: correctPathImpl; visible: Impl.valid && Impl.fileIntegrity; }
			Glow { anchors.fill: correctPathImpl; color: Impl.colors[4]; source: correctPathImpl; opacity: 0.3; visible: Impl.valid && Impl.fileIntegrity; }
			Private.ElevationWidgetPath { id: pathImpl; visible: Impl.valid && Impl.fileIntegrity; }
			Private.ElevationWidgetIntersections { id: intersectsImpl; visible: Impl.valid && Impl.fileIntegrity; }
			Glow { anchors.fill: intersectsImpl; color: Impl.colors[5]; source: intersectsImpl; opacity: 0.3; visible: Impl.valid && Impl.fileIntegrity; }
			Repeater
			{
				clip: false;
				model: pathModel;
				delegate: Delegates.ElevationWidgetPoint { }
			}
		}

		Private.ElevationWidgetLegend { id: legendImpl; anchors.fill: parent; visible: Impl.valid && Impl.fileIntegrity; }
		Private.ElevationWidgetMouseCross { id: mouseCrossImpl; anchors.fill: view; anchors.rightMargin: 10; visible: Impl.valid && Impl.fileIntegrity; }

		Keys.onPressed: { if (event.key === Qt.Key_Shift) { wheelHandler.shiftPressed = true } }
		Keys.onReleased: { if (event.key === Qt.Key_Shift) { wheelHandler.shiftPressed = false } }
		WheelHandler { id: wheelHandler;
			property real maxZoom: 1000;
			readonly property real zoom_sensivity: 3;
			property bool shiftPressed: false;

            onWheel: (event) =>
					 {
						 if(Impl.valid && Impl.fileIntegrity)
						 {
							 if(!shiftPressed) {
								 if(event.angleDelta.y > 0 && property_handler.zoomW <= maxZoom)
									property_handler.zoomW += (1 / zoom_sensivity) * property_handler.zoomW;
								 else if(event.angleDelta.y < 0 && property_handler.zoomW >= 1)
									property_handler.zoomW -= (1 / zoom_sensivity) * property_handler.zoomW;
							 } else {
								 if(event.angleDelta.y > 0)
									view.flick(1000, 0);
								 else if(event.angleDelta.y < 0)
									view.flick(-1000, 0);
							 }
						 }
					 }
		}
	}
	Text { id: warningLabel;
		anchors.fill: parent;
		color: Impl.colors[5];
		text: "ОТСУТСТВУЮТ ИСХОДНЫЕ ДАННЫЕ ПО ВЫСОТЕ";
		font.bold: true;
		font.pixelSize: width / 30;
		font.family: sFontFamily;
		horizontalAlignment: Text.AlignHCenter;
		verticalAlignment: Text.AlignVCenter;
		visible: !Impl.valid ? false : !Impl.fileIntegrity;
	}
	Text { id: warningLabel2;
		anchors.fill: parent;
		color: Impl.colors[4];
		text: "НЕ ЗАДАН МАРШРУТ";
		font.bold: true;
		font.pixelSize: width / 30;
		font.family: sFontFamily;
		horizontalAlignment: Text.AlignHCenter;
		verticalAlignment: Text.AlignVCenter;
		visible: !Impl.valid;
	}
}
