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
                    width: 400
                    height: 100
                    anchors.top: parent.top
                    anchors.left: parent.left
                    text: mainPageViewModel.error
                }
            }
        }

    Connections {
        target: mainPageViewModel
        function onChangeSpinnerState(running) {
            loader.running = running
        }
        function onOpenDialog() {
           dialog.open()
        }
    }

    Rectangle {
        anchors.fill : parent
        color: "transparent"
        //opacity: 0.8

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
                   text: "Put message here"
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
			//palette.buttonText: "red"
			background: Rectangle {
                            radius: 5
                            color: "#F6B4F4"
                            anchors.fill : parent
                            border.color: "black"
                            border.width: 1
                        }
			onClicked: {
				mainPageViewModel.setMessage(outputDialog.text)
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
            onClicked: {
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
            onClicked: {
                mainPageViewModel.boostButtonClicked()
            }
         }

         BusyIndicator {
            id: loader
            palette.dark: "white"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            running: false
         }
	}
}
