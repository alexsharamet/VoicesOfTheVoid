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
        opacity: 0.5

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
			text: mainPageViewModel.input
			background: Rectangle {
                            radius: 5
                            color: "transparent"
                            anchors.fill : parent
                            border.color: "#333"
                            border.width: 1
                        }

			readOnly: true
		}		

		TextField {
			id: output_dialog
			width: parent.width * 0.8
			height: parent.height * 0.2
			anchors.left: input_dialog.left
			anchors.top: input_dialog.bottom
			anchors.topMargin: 50

            background: Rectangle {
                radius: 5
                color: "transparent"
                anchors.fill : parent
                border.color: "#333"
                border.width: 1
            }
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
				mainPageViewModel.setMessage(output_dialog.text)
			}
		}

		Button {
            id: tune_button
            width: 200
            height: 50
            anchors.left: output_dialog.left
            anchors.top: output_dialog.bottom
            anchors.topMargin: 50
            text: "Tune"
            onClicked: {
                mainPageViewModel.tuneButtonCicked()
            }
        }

        Button {
            id: boost_button
            width: 200
            height: 50
            anchors.left: tune_button.right
            anchors.top: output_dialog.bottom
            anchors.leftMargin: 50
            anchors.topMargin: 50
            text: "Boost"
            onClicked: {
                mainPageViewModel.boostButtonClicked()
            }
         }
	}
}
