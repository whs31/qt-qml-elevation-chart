import QtQuick 2.15

Rectangle { id: gui_thread_freeze_indicator;
	property int i_Size: 50;
	property int u_BlinkFrequency: 100;

	width: i_Size;
	height: i_Size;
	radius: i_Size / 2;
	color: "#BF616A";
	SequentialAnimation {
		NumberAnimation {
			target: gui_thread_freeze_indicator;
			property: "opacity";
			to: 1;
			duration: u_BlinkFrequency;
		}
		NumberAnimation {
			target: gui_thread_freeze_indicator;
			property: "opacity";
			to: 0;
			duration: u_BlinkFrequency;
		}
		loops: Animation.Infinite;
		running: true;
	}
	SequentialAnimation {
		ColorAnimation {
			target: gui_thread_freeze_indicator;
			property: "color";
			to: "#D08770";
			duration: u_BlinkFrequency * 2;
		}
		ColorAnimation {
			target: gui_thread_freeze_indicator;
			property: "color";
			to: "#EBCB8B";
			duration: u_BlinkFrequency * 2;
		}
		ColorAnimation {
			target: gui_thread_freeze_indicator;
			property: "color";
			to: "#A3BE8C";
			duration: u_BlinkFrequency * 2;
		}
		ColorAnimation {
			target: gui_thread_freeze_indicator;
			property: "color";
			to: "#B48EAD";
			duration: u_BlinkFrequency * 2;
		}
		ColorAnimation {
			target: gui_thread_freeze_indicator;
			property: "color";
			to: "#BF616A";
			duration: u_BlinkFrequency * 2;
		}

		loops: Animation.Infinite;
		running: true;
	}
}
