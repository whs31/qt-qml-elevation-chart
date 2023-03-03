import QtQuick 2.15
import ElevationWidgetImpl 1.0

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
		console.log(Impl.colors[2]);
		ctx.lineWidth = 2;
		ctx.lineCap = "round";
		ctx.lineJoin = "round";

		// draw elevation profile
		ctx.moveTo(0, height);
		var p0 = Qt.point(0, 0);
		while(1)
		{
			let p = Impl.iterateOverRange(0, 1);
			if(p === Qt.point(-1, -1))
				break;
			ctx.beginPath();
			ctx.moveTo(p0.x * base.z_w, height - p0.y);
			ctx.lineTo(p.x * base.z_w, height - p.y);
			if(p0.x !== 0 && p0.y !== 0)
			{
				ctx.lineTo(p.x * base.z_w, height);
				ctx.lineTo(p0.x * base.z_w, height);
			}
			p0 = p;
			ctx.stroke();
			ctx.fill();
		}
	}
}
