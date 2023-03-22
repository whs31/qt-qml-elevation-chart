import QtQuick 2.15
import ElevationWidgetImpl 1.0

// @TODO: OpenGL

Canvas { id: elevationProfile;
	function refresh()
	{
		elevationProfile.requestPaint();
	}

	width: base.widthScaled;
	height: base.height;
	renderStrategy: Canvas.Threaded;
	clip: true;
	onPaint:
	{
		let ctx = getContext('2d');

		ctx.clearRect(0, 0, width, height);
		ctx.strokeStyle = String(Impl.colors[2]);
		ctx.fillStyle = String(Impl.colors[2]);
		ctx.lineWidth = 2;
		ctx.lineCap = "round";
		ctx.lineJoin = "round";

		// draw elevation profile
		ctx.beginPath();
		ctx.moveTo(0, height);
		while(1)
		{
			let p = Impl.iterateOverRange(0, 1);
			if(p === Qt.point(-1, -1))
				break;
			ctx.lineTo(p.x * base.z_w, height - p.y);
		}
		ctx.lineTo(width, height);
		ctx.lineTo(0, height);
		ctx.closePath();
		ctx.stroke();
		ctx.fill();
	}
}
