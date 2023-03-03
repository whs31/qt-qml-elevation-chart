import QtQuick 2.15
import ElevationWidgetImpl 1.0

Canvas { id: legend;
	z: 1;

	function refresh()
	{
		legend.requestPaint();
	}

	onPaint:
	{
		let ctx = getContext('2d');
		ctx.clearRect(0, 0, legend.width, legend.height);

		ctx.strokeStyle = Impl.colors[1];
		ctx.lineWidth = 5;

		ctx.beginPath();
		ctx.moveTo(0, 0);
		ctx.lineTo(0, height);
		ctx.lineTo(width, height);

		ctx.stroke();

		// OY scales
		context.moveTo(0, height);
		for(let k = 1; k < Impl.keyValues[8]; k++)
		{
			if(k % 5 == 0)
			{
				// grid
				ctx.globalAlpha = 0.35;
				ctx.font = "bold 12px sans-serif";
				ctx.strokeStyle = Qt.darker(Impl.colors[1], 1.5);
				ctx.lineWidth = 0.6;
				ctx.beginPath();
				ctx.moveTo(0, height - k * Impl.keyValues[10]);
				ctx.lineTo(width, height - k * Impl.keyValues[10]);
				ctx.stroke();
				ctx.globalAlpha = 1;

				// scales itself
				ctx.lineWidth = 3;
				ctx.strokeStyle = Impl.colors[1];
				ctx.beginPath();
				let val = Impl.keyValues[6] * k;
				let txt = Number(val).toFixed(0) + " м";
				ctx.fillStyle = Impl.colors[1];
				ctx.fillText(txt, 4, height - k * Impl.keyValues[10] - 4);
				ctx.moveTo(0, height - k * Impl.keyValues[10]);
				ctx.lineTo(40, height - k * Impl.keyValues[10]);
			}
			else
			{
				// grid
				ctx.globalAlpha = 0.3;
				ctx.font = "bold 12px sans-serif";
				ctx.strokeStyle = Qt.darker(Impl.colors[1], 1.5);
				ctx.lineWidth = 0.4;
				ctx.beginPath();
				ctx.moveTo(0, height - k * Impl.keyValues[10]);
				ctx.lineTo(width, height - k * Impl.keyValues[10]);
				ctx.stroke();
				ctx.globalAlpha = 1;

				// text on minor scales
				ctx.font = "bold 10px sans-serif";
				let val = Impl.keyValues[6] * k;
				let txt = Number(val).toFixed(0) + " м";
				ctx.fillStyle = Impl.colors[1];
				ctx.fillText(txt, 4, height - k * Impl.keyValues[10] - 4);

				// scales itself
				ctx.lineWidth = 2;
				ctx.strokeStyle = Impl.colors[1];
				ctx.beginPath();
				ctx.moveTo(0, height - k * Impl.keyValues[10]);
				ctx.lineTo(30, height - k * Impl.keyValues[10]);
			}
			ctx.stroke();
		}
	}
}
