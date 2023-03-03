import QtQuick 2.15
import "delegates" as Delegates
import ElevationWidgetImpl 1.0

Canvas { id: graph;
	property real flightPointSize: 15;
	width: base.widthScaled;
	height: base.height;

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
		ctx.lineWidth = 2;
		ctx.lineCap = "round";
		ctx.lineJoin = "round";

		// draw flight path
		ctx.moveTo(0, 0);

		if(Impl.path.length !== pathModel.count)
		{
			pathModel.clear();
		}
		for(let f = 0; f < Impl.path.length; f++)
		{
			console.log(Impl.path[f]);
			console.log(Impl.path);
			console.log(Impl.path.length);
			ctx.beginPath();
			//ctx.strokeStyle = (backend.pathErrorList[f]) ? warningColor : flightPathColor;
			ctx.strokeStyle = Impl.colors[3];
			ctx.lineWidth = 4;
			ctx.fillStyle = Impl.colors[3];
			if(f > 0) ctx.moveTo(Impl.path[f-1].x * base.z_w, Impl.path[f-1].y);
			ctx.lineTo(Impl.path[f].x * base.z_w, Impl.path[f].y);
			ctx.stroke();

			if(pathModel.count < Impl.path.length)
			{
				pathModel.append({
									 "m_x": Impl.path[f].x * base.z_w - flightPointSize / 2,
									 "m_y": Impl.path[f].y - flightPointSize / 2, flightPointSize, flightPointSize,
									 "m_distance": Impl.path[f].x / base.width * Impl.keyValues[1],
									 "m_elevation": Impl.keyValues[2] * Impl.keyValues[0] - Impl.path[f].y / base.height * Impl.keyValues[2] * Impl.keyValues[0],
									 "m_invalid": false
								 })
			}
			else
			{
				pathModel.setProperty(f, "m_x", Impl.path[f].x * base.z_w - flightPointSize / 2);
				pathModel.setProperty(f, "m_y", Impl.path[f].y - flightPointSize / 2, flightPointSize, flightPointSize);
				pathModel.setProperty(f, "m_distance", Impl.path[f].x / base.width * Impl.keyValues[1]);
				pathModel.setProperty(f, "m_elevation", Impl.keyValues[2] * Impl.keyValues[0] - Impl.path[f].y / base.height * Impl.keyValues[2] * Impl.keyValues[0]);
			}
		}
		ctx.closePath();
	}

	ListModel { id: pathModel; }
	Repeater
	{
		model: pathModel;
		delegate: Delegates.ElevationWidgetPoint { }
	}
}
