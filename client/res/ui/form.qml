import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Page {
    id: root
    width: 1280
    height: 860

    Rectangle {
        id: background
        anchors.fill : parent
        color: "transparent"

        Image {
            anchors.fill : parent
			fillMode: Image.PreserveAspectFit
            source: "qrc:/res/ui/back.jpeg"
        }
	}

	Item {
		id: main_window
		anchors.fill : parent

		TextField {
			id: input_dialog
			width: parent.width * 0.8
			height: parent.height * 0.3
			anchors.left: parent.left
			anchors.leftMargin: parent.width * 0.1
			anchors.top: parent.top
			anchors.topMargin: parent.height * 0.1
			opacity: 0.5
			text: mainPageViewModel.input

			readOnly: true
		}		


		TextField {
			id: output_dialog
			width: parent.width * 0.8
			height: parent.height * 0.2
			anchors.left: input_dialog.left
			anchors.top: input_dialog.bottom
			anchors.topMargin: 50
			opacity: 0.5
		}
		
		Button {
			id: send_button
			width: 200
			height: 50
			anchors.right: output_dialog.right
			anchors.top: output_dialog.bottom
			anchors.topMargin: 50
			text: "Send"
			onClicked: {
				mainPageViewModel.output = output_dialog.text
			}
		}
	}
}
