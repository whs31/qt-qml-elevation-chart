import QtQuick 2.15
import QtPositioning 5.12
import ElevationChart 1.0
import FPS 1.0

Rectangle { id: base;

	// ❮❮❮ input ❯❯❯
	property alias path: backend.geopath;
	property alias horizontalVelocity: backend.variometerHV;
	property alias rateOfClimb: backend.variometerROC;
	property alias rateOfDescend: backend.variometerROD;

	// ❮❮❮ settings ❯❯❯
	property alias logging: backend.logging;
	property bool showIndex: false;

	// ❮❮❮ colors ❯❯❯
	property alias backgroundColor: base.color;
	property alias chartColor: elevationProfile.color;
	property color flightPathColor: "#c4bb4b";
	property color successColor: "#7FD962";
	property color warningColor: "#e36209";
	property color errorColor: "#D95757";

	// ❮❮❮ tweaks ❯❯❯
	property alias pointSize: graph.flightPointSize;

	// ❮❮❮ slots ❯❯❯
	function updatePath()
	{
		backend.updateProfile();
	}

	focus: true;
	clip: true;
	color: "#222831";

	Component.onCompleted: { updatePath(); startTimer.start(); }
	Timer { id: startTimer; running: false; repeat: false; interval: 500; onTriggered: updatePath(); }

	Connections {
		target: backend;
		function onRequestRedraw() {
			if(backend.logging) console.info("<qplot-js> backend requesting redraw");
			requestAll();
		}
		function onRequestRedrawIntersects() {
			intersects.requestPaint();
		}

		function onRequestRedrawPath() {
			graph.requestPaint();
			legend.requestPaint();
			//elevationProfile.requestPaint(); // this is causing lag!
		}
	}

	ElevationChart { id: backend;
		geopath: QtPositioning.path([QtPositioning.coordinate(60, 30), QtPositioning.coordinate(60.2, 30.2)], 2);
		logging: true;
		offset: 0;
		pixelWidth: base.width;
		pixelHeight: base.height;
		zoomX: 1;
		verticalStretch: 1.2; // ❮❮❮ ⚠⚠⚠ do not touch ⚠⚠⚠ ❯❯❯
		variometerHV: 155; // ❮❮❮ m\s ❯❯❯
		variometerROC: 10; // ❮❮❮ m\s ❯❯❯
		variometerROD: 10; // ❮❮❮ m\s ❯❯❯

		Behavior on zoomX { NumberAnimation { duration: 250; } }
		onZoomXChanged: {
			if(zoomX > wheelHandler.maxZoom)
				zoomX = wheelHandler.maxZoom;
			if(zoomX < 1)
			{
				zoomX = 1;
			}
		}
		onScaleStepYChanged: { elevationProfile.requestPaint(); }
	}

	function requestAll()
	{
		if(backend.logging) console.info("<qplot-js> redrawing all");
		graph.requestPaint();
		legend.requestPaint();
		elevationProfile.requestPaint();
	}

	ListModel { id: pathModel; }
	Flickable { id: view;
		anchors.fill: parent;
		contentWidth: backend.pixelWidth * backend.zoomX;
		contentHeight: backend.pixelHeight;
		flickableDirection: Flickable.HorizontalAndVerticalFlick;
		interactive: false;
		boundsMovement: Flickable.StopAtBounds
		clip: true;
		pixelAligned: true;


		onMovementEnded: { requestAll(); }

		Canvas { id: elevationProfile;
			property color color: "#43a1ca";
			width: backend.pixelWidth * backend.zoomX;
			height: backend.pixelHeight;
			renderStrategy: Canvas.Threaded;
			clip: true;
			onPaint:
			{
				let ctx = getContext('2d');

				ctx.clearRect(0, 0, width * backend.zoomX, height);
				ctx.strokeStyle = chartColor;
				ctx.fillStyle = chartColor;
				ctx.lineWidth = 2;
				ctx.lineCap = "round";
				ctx.lineJoin = "round";

				// ❮❮❮ draw elevation profile ❯❯❯
				ctx.moveTo(0, height);
				var p0 = Qt.point(0, 0);
				while(1)
				{
					let p = backend.iterateOverRange(0, 1);
					if(p === Qt.point(-1, -1))
						break;
					ctx.beginPath();
					ctx.moveTo(p0.x * backend.zoomX, height - p0.y);
					ctx.lineTo(p.x * backend.zoomX, height - p.y);
					if(p0.x !== 0 && p0.y !== 0)
					{
						ctx.lineTo(p.x * backend.zoomX, height);
						ctx.lineTo(p0.x * backend.zoomX, height);
					}
					p0 = p;
					ctx.stroke();
					ctx.fill();
				}
			}
		}

		Canvas { id: graph;
			property real flightPointSize: 15;
			width: backend.pixelWidth * backend.zoomX;
			height: backend.pixelHeight;

			clip: true;

			onPaint:
			{
				let ctx = getContext('2d');

				ctx.clearRect(0, 0, graph.width * backend.zoomX, graph.height);
				ctx.strokeStyle = chartColor;
				ctx.fillStyle = chartColor;
				ctx.lineWidth = 2;
				ctx.lineCap = "round";
				ctx.lineJoin = "round";

				// ❮❮❮ draw flight path ❯❯❯
				ctx.moveTo(0, 0);

				if(backend.pathData.length !== pathModel.count)
				{
					pathModel.clear();
				}
				for(let f = 0; f < backend.pathData.length; f++)
				{
					ctx.beginPath();
					ctx.strokeStyle = flightPathColor;
					ctx.lineWidth = 4;
					ctx.fillStyle = flightPathColor;
					if(f > 0) ctx.moveTo(backend.pathData[f-1].x * (backend.zoomX), backend.pathData[f-1].y);
					ctx.lineTo(backend.pathData[f].x * (backend.zoomX), backend.pathData[f].y);
					ctx.stroke();

					if(pathModel.count < backend.pathData.length)
					{
						pathModel.append({
											"m_x": backend.pathData[f].x * (backend.zoomX) - flightPointSize / 2,
											"m_y": backend.pathData[f].y - flightPointSize / 2, flightPointSize, flightPointSize,
											"m_distance": backend.pathData[f].x / backend.pixelWidth * backend.realWidth,
											"m_elevation": backend.realHeight * backend.verticalStretch - backend.pathData[f].y / backend.pixelHeight * backend.realHeight * backend.verticalStretch,
											"m_invalid": false
										})
					}
					else
					{
						pathModel.setProperty(f, "m_x", backend.pathData[f].x * (backend.zoomX) - flightPointSize / 2);
						pathModel.setProperty(f, "m_y", backend.pathData[f].y - flightPointSize / 2, flightPointSize, flightPointSize);
						pathModel.setProperty(f, "m_distance", backend.pathData[f].x / backend.pixelWidth * backend.realWidth);
						pathModel.setProperty(f, "m_elevation", backend.realHeight * backend.verticalStretch - backend.pathData[f].y / backend.pixelHeight * backend.realHeight * backend.verticalStretch);
					}

					// variometer error display
					if(backend.pathErrorList[f])
					{
						ctx.beginPath();
						ctx.strokeStyle = warningColor;
						ctx.fillStyle = warningColor;
						ctx.setLineDash([16, 16]);
						ctx.lineWidth = 1.5;

						if(f > 0) ctx.moveTo(backend.pathData[f-1].x * (backend.zoomX), backend.pathData[f-1].y);
						ctx.lineTo(backend.pathData[f].x * (backend.zoomX), backend.pathErrorValueList[f]);
						ctx.lineTo(backend.pathData[f].x * (backend.zoomX), backend.pathData[f].y);
						ctx.moveTo(backend.pathData[f].x * (backend.zoomX), backend.pathErrorValueList[f]);
						ctx.closePath();
						ctx.globalAlpha = 0.5;
						ctx.stroke();
						ctx.globalAlpha = 0.25;
						ctx.fill();
						ctx.setLineDash([4000, 1]);
						ctx.globalAlpha = 1;
						pathModel.setProperty(f, "m_invalid", true);
					} else { pathModel.setProperty(f, "m_invalid", false); }
				}
				ctx.closePath();
				ctx.fillStyle = legend.color;
			}

			Canvas { id: intersects;
				width: backend.pixelWidth * backend.zoomX;
				height: backend.pixelHeight;
				renderStrategy: Canvas.Threaded;

				clip: true;
				onPaint:
				{
					let ctx = getContext('2d');

					ctx.clearRect(0, 0, intersects.width * backend.zoomX, intersects.height);
					ctx.strokeStyle = errorColor;
					ctx.fillStyle = errorColor;
					ctx.lineWidth = 6;
					ctx.lineCap = "round";
					ctx.lineJoin = "round";
					if(backend.intersectList.length > 0)
					{
						for(let f = 0; f < backend.intersectList.length; f++)
						{

							if(f % 2 === 1)
							{
								ctx.beginPath();
								if(f > 0) ctx.moveTo(backend.intersectList[f-1].x * (backend.zoomX), backend.intersectList[f-1].y);
								ctx.lineTo(backend.intersectList[f].x * (backend.zoomX), backend.intersectList[f].y);
								ctx.closePath();
								ctx.globalAlpha = 1;
								ctx.stroke();
								ctx.beginPath();
								ctx.lineTo(backend.intersectList[f].x * (backend.zoomX), backend.intersectList[f].y);
								ctx.lineTo(backend.intersectList[f].x * (backend.zoomX), height);
								ctx.lineTo(backend.intersectList[f-1].x * (backend.zoomX), height);
								ctx.lineTo(backend.intersectList[f-1].x * (backend.zoomX), backend.intersectList[f-1].y);
								ctx.closePath();
								ctx.globalAlpha = 0.5;
								ctx.fill();
							}
						}
					}
				}
			}

			Repeater
			{
				model: pathModel;
				delegate: FlightPoint { }
			}
		}
	}

	Canvas { id: legend;
		property color color: "#dae1e5";
		property alias offset: backend.offset;

		z: 1;
		anchors.fill: view;
		onPaint:
		{
			let ctx = getContext('2d');
			ctx.clearRect(0, 0, legend.width, legend.height);

			ctx.strokeStyle = legend.color;
			ctx.lineWidth = 5;

			ctx.beginPath();
			ctx.moveTo(0, 0);
			ctx.lineTo(0, height - legend.offset);
			ctx.lineTo(width, height - legend.offset);

			ctx.stroke();

			// ❮❮❮ OY scales ❯❯❯
			context.moveTo(0, height);
			for(let k = 1; k < backend.scaleCountY; k++)
			{
				if(k % 5 == 0)
				{
					// ❮❮❮ grid ❯❯❯
					ctx.globalAlpha = 0.35;
					ctx.font = "bold 12px sans-serif";
					ctx.strokeStyle = Qt.darker(legend.color, 1.5);
					ctx.lineWidth = 0.6;
					ctx.beginPath();
					ctx.moveTo(0, height - k * backend.scaleStepY);
					ctx.lineTo(width, height - k * backend.scaleStepY);
					ctx.stroke();
					ctx.globalAlpha = 1;

					// ❮❮❮ scales itself ❯❯❯
					ctx.lineWidth = 3;
					ctx.strokeStyle = legend.color;
					ctx.beginPath();
					let val = backend.scaleValueY * k;
					let txt = Number(val).toFixed(0) + " м";
					ctx.fillStyle = legend.color;
					ctx.fillText(txt, 4, height - k * backend.scaleStepY - 4);
					ctx.fillStyle = graph.color;
					ctx.moveTo(0, height - k * backend.scaleStepY);
					ctx.lineTo(40, height - k * backend.scaleStepY);
				}
				else
				{
					// ❮❮❮ grid ❯❯❯
					ctx.globalAlpha = 0.3;
					ctx.font = "bold 12px sans-serif";
					ctx.strokeStyle = Qt.darker(legend.color, 1.5);
					ctx.lineWidth = 0.4;
					ctx.beginPath();
					ctx.moveTo(0, height - k * backend.scaleStepY);
					ctx.lineTo(width, height - k * backend.scaleStepY);
					ctx.stroke();
					ctx.globalAlpha = 1;

					// ❮❮❮ text on minor scales ❯❯❯
					ctx.font = "bold 10px sans-serif";
					let val = backend.scaleValueY * k;
					let txt = Number(val).toFixed(0) + " м";
					ctx.fillStyle = legend.color;
					ctx.fillText(txt, 4, height - k * backend.scaleStepY - 4);
					ctx.fillStyle = graph.color;

					// ❮❮❮ scales itself ❯❯❯
					ctx.lineWidth = 2;
					ctx.strokeStyle = legend.color;
					ctx.beginPath();
					ctx.moveTo(0, height - k * backend.scaleStepY);
					ctx.lineTo(30, height - k * backend.scaleStepY);
				}
				ctx.stroke();
			}
		}

		Canvas { id: mouseCross;
			z: 1;
			anchors.fill: parent;
			onPaint: {
				// ❮❮❮ Mouse cross ❯❯❯
				let ctx = getContext('2d');

				ctx.clearRect(0, 0, legend.width, legend.height);
				ctx.beginPath();
				ctx.strokeStyle = Qt.darker(legend.color, 1.2);
				ctx.lineWidth = 1;
				ctx.globalAlpha = 0.6;
				ctx.moveTo(globalMouseArea.mouseX, 0);
				ctx.lineTo(globalMouseArea.mouseX, height);
				ctx.stroke();
			}
		}
		Rectangle { color: legend.color; width: 70; height: 15; anchors.top: legend.top; anchors.left: legend.left;
					Text { color: base.color; anchors.fill: parent; text: "ВЫСОТА"; font.bold: true;
						   horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; } }
		Rectangle { color: legend.color; width: 100; height: 15; anchors.bottom: legend.bottom; anchors.right: legend.right;
					anchors.bottomMargin: legend.offset;
					Text { color: base.color; anchors.fill: parent; text: "РАССТОЯНИЕ"; font.bold: true;
						   horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; } }
	}

	Keys.onPressed: { if (event.key === Qt.Key_Shift) { wheelHandler.shiftPressed = true } }
	Keys.onReleased: { if (event.key === Qt.Key_Shift) { wheelHandler.shiftPressed = false } }
	WheelHandler { id: wheelHandler;
		property real maxZoom: backend.realWidth / 1000;
		property real zoomConstInteger: 0;
		readonly property real zoom_sensivity: 3;
		property bool shiftPressed: false;

		onWheel: (event) =>
		{
			if(!shiftPressed) {
				if(event.angleDelta.y > 0 && backend.zoomX <= maxZoom)
					backend.zoomX += (1 / zoom_sensivity) * backend.zoomX;
				else if(event.angleDelta.y < 0 && backend.zoomX >= 1)
					backend.zoomX -= (1 / zoom_sensivity) * backend.zoomX;
			} else {
				if(event.angleDelta.y > 0)
					view.flick(500, 0);
				else if(event.angleDelta.y < 0)
					view.flick(-500, 0);
			}
		}
	}
	MouseArea { id: globalMouseArea;
		acceptedButtons: Qt.RightButton;
		anchors.fill: parent;
		hoverEnabled: true;
		onPositionChanged:
		{
			mouseCross.requestPaint();
		}
	}

	FPS { id: fpsWidget;
		anchors.left: parent.left;
		anchors.bottom: parent.bottom;
		width: 75;
		height: 25;
		opacity: 0.3;
		Text {
			anchors.centerIn: parent;
			font.bold: true;
			font.pixelSize: 20;
			text: Number(fpsWidget.fps).toFixed() + " FPS";
			color: "cyan";
		}
	}
}
