import QtQuick 2.15
import ElevationWidgetImpl 1.0

Canvas { id: mouseCross;
	z: 1;
	anchors.fill: parent;
	onPaint: {
		let ctx = getContext('2d');

		ctx.clearRect(0, 0, width, height);
		ctx.beginPath();
		ctx.strokeStyle = Impl.colors[1];
		ctx.fillStyle = Impl.colors[1];
		ctx.lineWidth = 2;
		ctx.globalAlpha = 0.6;
		ctx.moveTo(globalMouseArea.mouseX, 0);
		ctx.lineTo(globalMouseArea.mouseX, height);
		ctx.moveTo(globalMouseArea.mouseX + 8, height - 2);
		ctx.lineTo(globalMouseArea.mouseX, height - 10);
		ctx.lineTo(globalMouseArea.mouseX + 8, height - 18);
		let txt = Number((globalMouseArea.mouseX + view.visibleArea.xPosition * base.widthScaled) / 1000 / base.widthScaled * Impl.keyValues[1]).toFixed(1) + " км";
		let textDimensions = ctx.measureText(txt);
		ctx.lineTo(globalMouseArea.mouseX + 12 + textDimensions.width, height - 18);
		ctx.lineTo(globalMouseArea.mouseX + 12 + textDimensions.width, height - 2);
		ctx.lineTo(globalMouseArea.mouseX + 8, height - 2);
		ctx.stroke();
		ctx.fillText()
		ctx.fill();
		ctx.font = "bold 12px sans-serif";
		ctx.fillStyle = Impl.colors[0];
		ctx.fillText(txt, globalMouseArea.mouseX + 8, height - 6);
	}
}
