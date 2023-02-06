/*										Elevation Chart Widget
 * ПАРАМЕТРЫ КОМПОНЕНТА:
 *
 *
 * path: QGeoPath
	- путь, который будет отображен на графике
	(должен быть Q_PROPERTY с сигналом NOTIFY,
	чтобы динамически обновляться)

 * color: color
	- цвет фона виджета

 * legend.color: color
	- цвет легенды графика
	(оси, текст)

 * legend.offset: int
	- отступ графика от нижнего края виджета

 * graph.color: color
	- цвет графика

 * graph.verticalStretch: real
	- растяжение графика по высоте
	(1 означает, что максимальная высота на графике будет
	равняться высоте оси Y. При коэффициентах более 1 ось
	Y будет выше максимальной возвышенности на графике в N
	раз, при коэффициентах менее 1 - часть графика будет
	выше оси Y)



 * ВСПОМОГАТЕЛЬНЫЕ ПАРАМЕТРЫ:
 *
 *
 * logging: bool
	- флаг вывода логов виджета в консоль отладки

 * grid.color: color
	- цвет сетки
	(по умолчанию равен цвету легенды,
	затемненному в 3 раза)
*/

import QtQuick 2.15
import QtPositioning 5.12
import QtQuick.Controls 2.15
import ElevationChart 1.0

Rectangle {
	id: base;
	color: "#121617";
	property alias path: backend.geopath;
	property alias logging: backend.logging;

	focus: true;
	clip: true;

	Connections {
		target: backend;
		function onRequestRedraw() {
			if(backend.logging)
				console.error("Requesting redraw");
			legend.requestPaint();
			graph.requestPaint();
			grid.requestPaint();
		}
	}
	ElevationChart
	{
		id: backend;
		geopath: QtPositioning.path([QtPositioning.coordinate(60, 30), QtPositioning.coordinate(60.2, 30.2)], 2);
		logging: true;
		offset: 30;
		pixelWidth: graph.width;
		pixelHeight: graph.height;
		zoomX: 1;
		verticalStretch: 1.5;

		Behavior on zoomX { NumberAnimation { duration: 100; easing.type: Easing.InOutCubic; } }
		onZoomXChanged: {
			if(zoomX > wheelHandler.maxZoom)
				zoomX = wheelHandler.maxZoom;
			if(zoomX < 1)
			{
				zoomX = 1;
				horizontalScrollBar.position = 0;
			}
			legend.requestPaint();
			graph.requestPaint();
			grid.requestPaint();
		}
	}

	MouseArea
	{
		//property real cursorRelativePositionX: 0;

		id: mouseArea;
		anchors.fill: parent;
		propagateComposedEvents: true;
		hoverEnabled: true;
//		onMouseXChanged:
//		{
//			cursorRelativePositionX = mouseX / (graph.width);
//		}
	}

	Keys.onPressed: {
		if (event.key === Qt.Key_Shift) {
			wheelHandler.shiftPressed = true
		}
	}
	Keys.onReleased: {
		if (event.key === Qt.Key_Shift) {
			wheelHandler.shiftPressed = false
		}
	}

	WheelHandler {
		id: wheelHandler;
		property real maxZoom: backend.realWidth;
		property real zoomConstInteger: 0;
		readonly property real zoom_sensivity: 3;
		property real zoompixelwidth: graph.width * (backend.zoomX);
		property real zoomTranslation: (1 - backend.zoomX) * horizontalScrollBar.scrollCoeff * graph.width / 2; // 0 = left border | 1 = center | 2 = right border
		property bool shiftPressed: false;

		onWheel: (event) =>
				 {
					 if(!shiftPressed)
					 {
						 if(event.angleDelta.y > 0 && mouseArea.containsMouse && backend.zoomX <= maxZoom)
						 {
							 backend.zoomX += (1 / zoom_sensivity) * backend.zoomX;
						 }
						 else if(event.angleDelta.y < 0 && mouseArea.containsMouse && backend.zoomX >= 1)
						 {
							 backend.zoomX -= (1 / zoom_sensivity) * backend.zoomX;
						 }
					 }
					 else
					 {
						 if(event.angleDelta.y > 0 && mouseArea.containsMouse && horizontalScrollBar.position < 1 - horizontalScrollBar.size / 2)
						 {
							 horizontalScrollBar.position += 0.03 * horizontalScrollBar.size;
						 }
						 else if(event.angleDelta.y < 0 && mouseArea.containsMouse && horizontalScrollBar.position > -0.1)
						 {
							 horizontalScrollBar.position -= 0.03 * horizontalScrollBar.size;
						 }
					 }
				 }
	}
	Canvas
	{
		id: legend;
		property color color: "#dae1e5";
		property alias offset: backend.offset;

		z: 1;
		anchors.fill: parent;
		onPaint:
		{
			let ctx = getContext('2d');

			ctx.strokeStyle = legend.color;
			ctx.lineWidth = 2;

			ctx.beginPath();
			ctx.moveTo(1, 0);
			ctx.lineTo(1, height - legend.offset);
			ctx.lineTo(width, height - legend.offset);

			ctx.stroke();
		}
		Rectangle { color: legend.color; width: 90; height: 15; anchors.top: legend.top; anchors.left: legend.left;
					Text { color: base.color; anchors.fill: parent; text: "ВЫСОТА, м"; font.bold: true;
						   horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; } }
		Rectangle { color: legend.color; width: 100; height: 15; anchors.bottom: legend.bottom; anchors.right: legend.right;
					anchors.bottomMargin: legend.offset;
					Text { color: base.color; anchors.fill: parent; text: "РАССТОЯНИЕ"; font.bold: true;
						   horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; } }
	}

	Canvas
	{
		id: graph;
		property color color: "#43a1ca";
		property alias verticalStretch: backend.verticalStretch;

		anchors.fill: legend;
		anchors.bottomMargin: legend.offset;

		antialiasing: true;
		smooth: true;
		onPaint:
		{
			let ctx = getContext('2d');

			ctx.clearRect(0, 0, graph.width, graph.height);
			ctx.strokeStyle = graph.color;
			ctx.fillStyle = graph.color;
			ctx.lineWidth = 2;
			ctx.lineCap = "round";
			ctx.lineJoin = "round";

			ctx.setTransform(backend.zoomX, 0, 0, 1, wheelHandler.zoomTranslation, 0); //scaleX : 0 : 0 : scaleY : translateX : translateY
			ctx.moveTo(0, height);

			var p0 = Qt.point(0, 0);
			while(1)
			{
				let p = backend.iterateOverRange(0, 1);
				if(p === Qt.point(-1, -1))
					break;
				ctx.beginPath();
				ctx.lineTo(p.x, height - p.y);
				if(p0.x !== 0 && p0.y !== 0)
				{
					ctx.lineTo(p0.x - 0.3, height - p0.y);
					ctx.lineTo(p0.x - 0.7, height);
					ctx.lineTo(p.x, height);
					ctx.lineTo(p.x, height - p.y);
				}
				ctx.closePath();
				ctx.fill();
				p0 = p;
			}

			ctx.stroke();
		}
	}

	Canvas
	{
		id: grid;
		anchors.fill: graph;
		property color color: Qt.darker(legend.color, 2); // can be overriden

		onPaint:
		{
			let ctx = getContext('2d');

			ctx.clearRect(-wheelHandler.zoompixelwidth / 2, 0, wheelHandler.zoompixelwidth * 2, grid.height);
			ctx.strokeStyle = grid.color;
			ctx.fillStyle = legend.color;
			ctx.lineWidth = 1;
			ctx.lineCap = "round";
			ctx.lineJoin = "round";

			ctx.setTransform(1, 0, 0, 1, wheelHandler.zoomTranslation, 0);

			// OX scales (recursive)
			ctx.moveTo(0, height);
			let interval = backend.scaleStepX * backend.zoomX;
			wheelHandler.zoomConstInteger = 0;
			while(interval >= 80)
			{
				wheelHandler.zoomConstInteger++;
				let mod5 = 0;
				for(let inner_interval = 0; inner_interval < wheelHandler.zoompixelwidth * Math.min(horizontalScrollBar.position + horizontalScrollBar.size, 1); inner_interval += interval)
				{
					if(inner_interval < wheelHandler.zoompixelwidth * horizontalScrollBar.position)
					{ mod5++; continue; }

					if(mod5++ % 5 == 0)
					{
						// grid on major scales ( /= 5 )
						ctx.globalAlpha = 0.5;
						ctx.font = "bold 12px sans-serif";
						ctx.strokeStyle = Qt.darker(legend.color, 1.25);
						ctx.lineWidth = 1;
						ctx.setLineDash([16, 16]);
						ctx.beginPath();
						ctx.moveTo(inner_interval, 0);
						ctx.lineTo(inner_interval, height);
						ctx.stroke();
						ctx.globalAlpha = 1;

						// scales itself
						ctx.setLineDash([4000, 1]);
						ctx.lineWidth = 3;
						ctx.strokeStyle = legend.color;
						ctx.beginPath();
						let val = (backend.scaleValueX * (mod5 - 1) / 5) / Math.pow(5, wheelHandler.zoomConstInteger - 2);
						let txt = val >= 10000 ? Number(val / 1000).toFixed(1) + " км" : Number(val).toFixed(0) + " м";
						ctx.fillStyle = legend.color;
						ctx.fillText(txt, inner_interval + 4, graph.height - 5);
						ctx.fillStyle = graph.color;
						ctx.moveTo(inner_interval, height);
						ctx.lineTo(inner_interval, height - 15);

					} else {
						// grid on minor scales
						ctx.globalAlpha = 0.5;
						ctx.font = "bold 10px sans-serif";
						ctx.strokeStyle = Qt.darker(legend.color, 1.5);
						ctx.lineWidth = 0.75;
						ctx.setLineDash([8, 16]);
						ctx.beginPath();
						ctx.moveTo(inner_interval, 0);
						ctx.lineTo(inner_interval, height);
						ctx.stroke();
						ctx.globalAlpha = 1;

						// minor scales itself
						ctx.setLineDash([4000, 1]);
						ctx.lineWidth = 2;
						ctx.strokeStyle = legend.color;
						ctx.beginPath();
						ctx.moveTo(inner_interval, height);
						ctx.lineTo(inner_interval, height - 10);
					}
					ctx.stroke();
					ctx.setLineDash([4000, 1]);
				}
				interval /= 5;
			}

			ctx.setTransform(1, 0, 0, 1, 0, 0);
			// OY scales
			context.moveTo(0, height);
			for(let k = 1; k < backend.scaleCountY; k++)
			{
				if(k % 5 == 0)
				{
					// grid
					ctx.globalAlpha = 0.5;
					ctx.font = "bold 12px sans-serif";
					ctx.strokeStyle = Qt.darker(legend.color, 1.5);
					ctx.lineWidth = 0.75;
					ctx.setLineDash([8, 16]);
					ctx.beginPath();
					ctx.moveTo(0, height - k * backend.scaleStepY);
					ctx.lineTo(width, height - k * backend.scaleStepY);
					ctx.stroke();
					ctx.globalAlpha = 1;

					// scales itself
					ctx.setLineDash([4000, 1]);
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
					// text on minor scales
					ctx.font = "bold 10px sans-serif";
					let val = backend.scaleValueY * k;
					let txt = Number(val).toFixed(0) + " м";
					ctx.fillStyle = legend.color;
					ctx.fillText(txt, 4, height - k * backend.scaleStepY - 4);
					ctx.fillStyle = graph.color;

					// scales itself
					ctx.setLineDash([4000, 1]);
					ctx.lineWidth = 2;
					ctx.strokeStyle = legend.color;
					ctx.beginPath();
					ctx.moveTo(0, height - k * backend.scaleStepY);
					ctx.lineTo(30, height - k * backend.scaleStepY);
				}
				ctx.stroke();
			}
		}
	}

	ScrollBar {
		property real scrollCoeff: (position + size * (position + size / 2)) * 2;

		id: horizontalScrollBar;
		hoverEnabled: true;
		active: true;
		orientation: Qt.Horizontal;
		size: graph.width / wheelHandler.zoompixelwidth;
		policy: ScrollBar.AlwaysOn;
		anchors.left: parent.left;
		anchors.right: parent.right;
		anchors.bottom: parent.bottom;

		contentItem: Rectangle {
			implicitWidth: 6;
			color: graph.color;
			radius: 5;
		}
		onScrollCoeffChanged:
		{
			legend.requestPaint();
			graph.requestPaint();
			grid.requestPaint();
		}
		Behavior on position { NumberAnimation { duration: 100; } }
	}
}
