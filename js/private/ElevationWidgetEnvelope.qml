import QtQuick 2.15
import ElevationWidgetImpl 1.0

// @TODO: OpenGL

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
		ctx.lineCap = "round";
		ctx.lineJoin = "round";
		ctx.lineWidth = 3;

		// draw flight path
		ctx.moveTo(0, 0);

		if(Impl.path.length !== pathModel.count)
		{
			pathModel.clear();
		}
		ctx.strokeStyle = Impl.colors[6];
		ctx.fillStyle = Impl.colors[6];
		ctx.beginPath();
		for(let f = 0; f < Impl.envelope.length; f++)
		{
			if(f > 0) ctx.moveTo(Impl.envelope[f-1].x * base.z_w, Impl.envelope[f-1].y);
			ctx.lineTo(Impl.envelope[f].x * base.z_w, Impl.envelope[f].y);
			ctx.ellipse(Impl.envelope[f].x * base.z_w - 5, Impl.envelope[f].y - 5, 10, 10);
		}
		ctx.stroke();
		ctx.fill();
		ctx.closePath();
	}
}
