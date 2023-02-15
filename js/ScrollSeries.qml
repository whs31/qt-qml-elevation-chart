import QtQuick 2.15

Rectangle { id: base;
	// ❮❮❮ input ❯❯❯
	//required property var points;

	// ❮❮❮ settings ❯❯❯
	property int pointsCount: 100;

	// ❮❮❮ colors ❯❯❯
	property alias backgroundColor: base.color;

	color: "#222831";
	clip: true;

	onWidthChanged: mainCanvas.q

	Canvas { id: mainCanvas;

	}
}
