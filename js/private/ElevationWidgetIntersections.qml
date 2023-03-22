import QtQuick 2.15
import ElevationWidgetImpl 1.0

// @TODO: OpenGL

Canvas { id: intersects;
	function refresh()
	{
		intersects.requestPaint();
	}

	width: base.widthScaled;
	height: base.height;
	renderStrategy: Canvas.Threaded;

	clip: true;
	onPaint:
	{
		let ctx = getContext('2d');

		ctx.clearRect(0, 0, width * base.z_w, height);
		ctx.strokeStyle = Impl.colors[5];
		ctx.fillStyle = Impl.colors[5];
		ctx.lineWidth = 6;
		ctx.lineCap = "round";
		ctx.lineJoin = "round";
		if(Impl.intersections.length > 0)
		{
			for(let f = 0; f < Impl.intersections.length; f++)
			{

				if(f % 2 === 1)
				{
					ctx.beginPath();
					if(f > 0) ctx.moveTo(Impl.intersections[f-1].x * base.z_w, Impl.intersections[f-1].y);
					ctx.lineTo(Impl.intersections[f].x * base.z_w, Impl.intersections[f].y);
					ctx.closePath();
					ctx.globalAlpha = 1;
					ctx.stroke();
					ctx.beginPath();
					ctx.lineTo(Impl.intersections[f].x * base.z_w, Impl.intersections[f].y);
					ctx.lineTo(Impl.intersections[f].x * base.z_w, height);
					ctx.lineTo(Impl.intersections[f-1].x * base.z_w, height);
					ctx.lineTo(Impl.intersections[f-1].x * base.z_w, Impl.intersections[f-1].y);
					ctx.closePath();
					ctx.globalAlpha = 0.5;
					ctx.fill();
				}
			}
		}
	}
}
