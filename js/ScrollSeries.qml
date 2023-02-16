import QtQuick 2.15

Rectangle { id: base;
	// ❮❮❮ input ❯❯❯
	property var points: [];

	// ❮❮❮ settings ❯❯❯
	property int pointsCount: 100;

	// ❮❮❮ colors ❯❯❯
	property alias backgroundColor: base.color;
	property color graphColor: "#5a32a3";
	property color legendColor: "#dae1e5";

	color: "#222831";
	clip: true;

	onWidthChanged: mainCanvas.requestPaint();
	onHeightChanged: mainCanvas.requestPaint();
	onPointsChanged:
	{
		for(let i = 0; i < points.length; i++)
		{
			if(points[i] > mainCanvas.yMax)
				mainCanvas.yMax = points[i];
			if(i >= pointsCount)
				break;
		}
		mainCanvas.requestPaint();
	}

	Canvas { id: mainCanvas;
		clip: true;
		anchors.fill: parent;

		property real yMax: 1;

		onPaint:
		{
			let ctx = getContext('2d');

			ctx.clearRect(0, 0, mainCanvas.width, mainCanvas.height);

			ctx.beginPath();
			ctx.globalAlpha = 0.5;
			ctx.font = "bold 12px sans-serif";
			ctx.strokeStyle = Qt.darker(legendColor, 1.5);
			ctx.lineWidth = 0.75;
			ctx.setLineDash([16, 8]);
			ctx.lineCap = "round";
			ctx.lineJoin = "round";
			ctx.moveTo(0, 0);
			let scaleStepY = mainCanvas.height / yMax * Math.pow(10, Math.round((Math.log(yMax) / Math.log(10)) - 1));
			for(let yy = 0; yy < mainCanvas.height; yy += scaleStepY)
			{
				ctx.moveTo(0, yy);
				ctx.lineTo(mainCanvas.width, yy);
			}
			let scaleStepX = mainCanvas.width / pointsCount * Math.pow(10, Math.round((Math.log(yMax) / Math.log(10)) - 1));
			for(let xx = 0; xx < mainCanvas.width; xx += scaleStepX)
			{
				ctx.moveTo(xx, 0);
				ctx.lineTo(xx, mainCanvas.height);
			}
			ctx.stroke();

			ctx.beginPath();
			ctx.globalAlpha = 0.7;
//			ctx.shadowOffsetX    = 10;
//			ctx.shadowOffsetY    = -15;
//			ctx.shadowBlur    = 1;
//			ctx.shadowColor    = "rgba(0,0,0,0.2)";
			ctx.fillStyle = graphColor;
			for(let pp = 0; pp < points.length; pp++)
			{
				if(pp > 0) ctx.moveTo((pp-1) / pointsCount * mainCanvas.width, height - (points[pp-1] / yMax * mainCanvas.height));
				ctx.lineTo((pp) / pointsCount * mainCanvas.width, height - (points[pp] / yMax * mainCanvas.height));
				if(pp > 0)
				{
					ctx.lineTo((pp) / pointsCount * mainCanvas.width, height);
					ctx.lineTo((pp-1) / pointsCount * mainCanvas.width, height);
					ctx.lineTo((pp-1) / pointsCount * mainCanvas.width, height - (points[pp-1] / yMax * mainCanvas.height));
				}

				if(pp > pointsCount) break;
			}

			ctx.fill();
		}
	}
}
