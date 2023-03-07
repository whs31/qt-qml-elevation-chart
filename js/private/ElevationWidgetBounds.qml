import QtQuick 2.15
import ElevationWidgetImpl 1.0

Canvas { id: graph;
	width: base.widthScaled;
	height: base.height;
	renderStrategy: Canvas.Threaded;

	clip: true;

	function refresh()
	{
		graph.requestPaint();
	}

	Component.onCompleted: refresh();

	onPaint:
	{
		let ctx = getContext('2d');

		ctx.clearRect(0, 0, base.widthScaled, base.height);
		ctx.lineWidth = 1;
		ctx.setLineDash([1, 1]);

		// draw flight path
		ctx.moveTo(0, 0);

		for(let f = 0; f < Impl.bounds.length; f++)
		{
			ctx.beginPath();
			ctx.strokeStyle = Impl.colors[4];
			ctx.lineWidth = 4;
			ctx.fillStyle = Impl.colors[4];
			if(f > 0) ctx.moveTo(Impl.bounds[f-1].x * base.z_w, Impl.bounds[f-1].y);
			ctx.lineTo(Impl.bounds[f].x * base.z_w, Impl.bounds[f].y);
			ctx.stroke();
		}
	}
}
