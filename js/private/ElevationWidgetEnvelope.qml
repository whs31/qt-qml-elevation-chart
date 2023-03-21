import QtQuick 2.15
import ElevationWidgetImpl 1.0

Canvas { id: graph;
	width: base.widthScaled;
	height: base.height;

	clip: true;

	function refresh()
	{
		graph.requestPaint();
	}

	SequentialAnimation {
		PropertyAnimation {
			target: graph;
			property: "opacity";
			to: 0;
			duration: 500;
			easing.type: Easing.InOutQuad;
		}
		PropertyAnimation {
			target: graph;
			property: "opacity";
			to: 1.0;
			duration: 500;
			easing.type: Easing.InOutQuad;
		}
		loops: Animation.Infinite;
		running: true;
		Component.onCompleted: start();
	}

	Component.onCompleted: refresh();

	onPaint:
	{
		let ctx = getContext('2d');

		ctx.clearRect(0, 0, base.widthScaled, base.height);
		ctx.setLineDash([2, 2]);
		ctx.lineWidth = 2;
		ctx.lineCap = "round";
		ctx.lineJoin = "round";

		// draw flight path
		ctx.moveTo(0, 0);

		if(Impl.path.length !== pathModel.count)
		{
			pathModel.clear();
		}
		for(let f = 0; f < Impl.envelope.length; f++)
		{
			ctx.beginPath();
			ctx.strokeStyle = Impl.colors[6];
			ctx.lineWidth = 4;
			ctx.fillStyle = Impl.colors[6];
			if(f > 0) ctx.moveTo(Impl.envelope[f-1].x * base.z_w, Impl.envelope[f-1].y);
			ctx.lineTo(Impl.envelope[f].x * base.z_w, Impl.envelope[f].y);
			ctx.stroke();
		}
		ctx.closePath();
	}
}
