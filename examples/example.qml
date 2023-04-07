import QtQuick 2.15

Rectangle { id: root;
	color: "#1b1f23";

	ElevationWidget {
		anchors.fill: parent;
	}

	Rectangle { id: gui_thread_freeze_indicator;
		anchors.left: parent.left;
		anchors.top: parent.top;
		anchors.margins: 50;
		width: 50;
		height: 50;
		radius: 25;
		color: "#BF616A";
		SequentialAnimation {
			NumberAnimation {
				target: gui_thread_freeze_indicator;
				property: "opacity";
				to: 1;
				duration: 250;
			}
			NumberAnimation {
				target: gui_thread_freeze_indicator;
				property: "opacity";
				to: 0;
				duration: 250;
			}
			loops: Animation.Infinite;
			running: true;
		}
		SequentialAnimation {
			ColorAnimation {
				target: gui_thread_freeze_indicator;
				property: "color";
				to: "#D08770";
				duration: 250;
			}
			ColorAnimation {
				target: gui_thread_freeze_indicator;
				property: "color";
				to: "#EBCB8B";
				duration: 250;
			}
			ColorAnimation {
				target: gui_thread_freeze_indicator;
				property: "color";
				to: "#A3BE8C";
				duration: 250;
			}
			ColorAnimation {
				target: gui_thread_freeze_indicator;
				property: "color";
				to: "#B48EAD";
				duration: 250;
			}
			ColorAnimation {
				target: gui_thread_freeze_indicator;
				property: "color";
				to: "#BF616A";
				duration: 250;
			}

			loops: Animation.Infinite;
			running: true;
		}
	}
}
