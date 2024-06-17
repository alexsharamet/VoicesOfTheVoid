import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
//import QtQuick.Controls.Styles 1.4
//import QtQuick.Controls.Basic

Page {
    id: root
    width: 1280
    height: 800

    Dialog {
            id: dialog
            anchors.centerIn : Overlay.overlay
            width: 400
            height: 200
            standardButtons: Dialog.Ok
            contentItem: Item {
                Label {
                    id: dialogText
                    width: 400
                    height: 100
                    anchors.top: parent.top
                    anchors.left: parent.left
                }
            }
        }

    Connections {
        target: mainPageViewModel
        function onChangeSpinnerState(running) {
            loader.running = running
        }
        function onOpenDialog(text) {
            dialogText.text = text
            dialog.open()
        }
    }

    Rectangle {
        anchors.fill : parent
        color: "transparent"

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
            color: "white"
            visible: mainPageViewModel.debug
		}

        ScrollView {
            id: inputDialogView
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            width: parent.width * 0.8
            height: parent.height * 0.3
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.1
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1

            TextArea {
                 id: inputDialog

                 text: mainPageViewModel.input
                 color: "orange"
                 background: Rectangle {
                             radius: 5
                             color: "black"
                             opacity: 0.4
                             anchors.fill : parent
                             border.color: "white"
                             border.width: 2
                         }


                 readOnly: true
            }
		}

        ScrollView {
            id: outputDialogView
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            width: parent.width * 0.8
            height: parent.height * 0.2
            anchors.left: inputDialogView.left
            anchors.top: inputDialogView.bottom
            anchors.topMargin: 100

            TextArea {
                id: outputDialog

                width: parent.width
                color: "orange"
                background: Rectangle {
                    radius: 5
                    color: "black"
                    opacity: 0.4
                    anchors.fill : parent
                    border.color: "white"
                    border.width: 2
                }

                Label {
                   id: outputDialogLabelHelper
                   anchors.left: parent.left
                   anchors.top: parent.top

                   anchors.leftMargin: 10
                   anchors.topMargin: 10

                   color: "orange"
                   text: "Enter your message here:"
                   visible: true

                   background: Rectangle {
                       color: "transparent"
                       anchors.fill : parent
                       border.width: 0
                   }
                }

                Button {
                        id: outputDialogHelper
                        anchors.fill: parent

                        visible: true
                        background: Rectangle {
                            color: "transparent"
                            anchors.fill : parent
                            border.width: 0
                        }
                        onClicked: {
                            outputDialogLabelHelper.visible = false
                            outputDialogHelper.visible = false
                            outputDialog.forceActiveFocus()
                        }
                    }
            }
		}

		Button {
			id: sendButton
			width: 200
			height: 50
			anchors.right: outputDialogView.right
			anchors.top: outputDialogView.bottom
			anchors.topMargin: 50
			text: "Send"
			palette.buttonText: "black"
			background: Rectangle {
                            radius: 5
                            color: "#F6B4F4"
                            anchors.fill : parent
                            border.color: "black"
                            border.width: 1
                        }
			onClicked: {
			    boostButton.enabled = true
                boostButtonBack.color = "white"
                tuneButton.enabled = true
                tuneButtonBack.color = "white"
				mainPageViewModel.setMessage(outputDialog.text)
			    outputDialog.text = ""
			}
		}

		Button {
            id: tuneButton
            width: 200
            height: 50
            anchors.left: outputDialogView.left
            anchors.top: outputDialogView.bottom
            anchors.topMargin: 50
            text: "Tune"
            palette.buttonText: "black"
            background: Rectangle {
                id: tuneButtonBack
                radius: 5
                color: "white"
                anchors.fill : parent
                border.color: "black"
                border.width: 1
            }
            onClicked: {
                boostButton.enabled = false
                boostButtonBack.color = "grey"
                tuneButton.enabled = false
                tuneButtonBack.color = "grey"
                outputDialog.text = ""
                outputDialogLabelHelper.visible = true
                outputDialogHelper.visible = true
                sendButton.forceActiveFocus()
                mainPageViewModel.tuneButtonCicked()
            }
        }

        Button {
            id: boostButton
            width: 200
            height: 50
            anchors.left: tuneButton.right
            anchors.top: outputDialogView.bottom
            anchors.leftMargin: 50
            anchors.topMargin: 50
            text: "Boost"
            palette.buttonText: "black"
            background: Rectangle {
                id: boostButtonBack
                radius: 5
                color: "white"
                anchors.fill : parent
                border.color: "black"
                border.width: 1
            }
            onClicked: {
                boostButton.enabled = false
                boostButtonBack.color = "grey"
                tuneButton.enabled = false
                tuneButtonBack.color = "grey"
                mainPageViewModel.boostButtonClicked()
            }
         }

        BusyIndicator {
            id: loader
            palette.dark: "white"
            width: 70
            height: 70
            anchors.centerIn: parent
            running: false
        }
	}
}
