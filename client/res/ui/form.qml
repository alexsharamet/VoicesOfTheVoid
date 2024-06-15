import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
//import QtQuick.Controls.Basic

Page {
    id: root
    width: 1280
    height: 860

    Connections {
        target: mainPageViewModel
        function onChangeSpinnerState(running) {
            loader.running = running
        }
    }

    Rectangle {
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
		id: mainWindow
		anchors.fill : parent

		Label {
		    anchors.left: parent.left
		    anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            text: mainPageViewModel.strategy
		}

		TextArea {
			id: inputDialog
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

		TextArea {
			id: outputDialog
			width: parent.width * 0.8
			height: parent.height * 0.2
			anchors.left: inputDialog.left
			anchors.top: inputDialog.bottom
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
			id: sendButton
			width: 200
			height: 50
			anchors.right: outputDialog.right
			anchors.top: outputDialog.bottom
			anchors.topMargin: 50
			text: "Send"
			onClicked: {
				mainPageViewModel.setMessage(outputDialog.text)
			}
		}

		Button {
            id: tuneButton
            width: 200
            height: 50
            anchors.left: outputDialog.left
            anchors.top: outputDialog.bottom
            anchors.topMargin: 50
            text: "Tune"
            onClicked: {
                mainPageViewModel.tuneButtonCicked()
            }
        }

        Button {
            id: boostButton
            width: 200
            height: 50
            anchors.left: tuneButton.right
            anchors.top: outputDialog.bottom
            anchors.leftMargin: 50
            anchors.topMargin: 50
            text: "Boost"
            onClicked: {
                mainPageViewModel.boostButtonClicked()
            }
         }

         BusyIndicator {
            id: loader
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            running: false
         }
	}
}
