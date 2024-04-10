import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "../component"
import MOZA.DashboardEditor 1.0

Page {
    id: root
    leftPadding: 0
    rightPadding: 8

    header: TitleBar {
        width: parent.width
        height: 48
        DragHandler {
            target: null
            onActiveChanged: {
                if (active) mainWindow.startSystemMove()
            }
        }
    }

    contentItem: Pane {
        id: contentScrollView
        leftPadding: 24
        rightPadding: 36
        topPadding: 20
        bottomPadding: 20
        clip: true

        ColumnLayout {
            width: contentScrollView.availableWidth
            spacing: 12

            RowLayout {
                spacing: 12
                MozaButton {
                    text: "展开"
                    onClicked: {
                        if (showRec.state === "SHOW") {
                            showRec.state = "HIDE"
                        } else {
                            showRec.state = "SHOW"
                        }
                    }
                }
                MozaButton {
                    text: "异步"
                    onClicked: {
                        new Promise(function (resolve, reject) {
                            console.log("开始")
                            resolve();
                        }).then(function () {
                            return new Promise(function (resolve, reject) {
                                console.log("中间")
                                resolve();
                            });
                        }).then(function () {
                            console.log("结束")
                            resolve();
                        });
                    }
                }
            }

            RowLayout {
                ColumnLayout {
                    MozaTreeView {
                        id: mozaTreeView
                        Layout.preferredWidth: 320
                        Layout.fillHeight: true
                        treeModel: MozaEditorFileManager.getModelDataFromFile("D:/MyFiles/test/editor.json")
                    }
                    Rectangle {
                        color: 'red'
                        opacity: 0.5
                        radius: 4
                        Layout.preferredWidth: 320
                        Layout.preferredHeight: 50
                    }
                }

                ColumnLayout {
                    spacing: 16
                    Rectangle {
                        id: showRec
                        state: "SHOW"
                        states: [
                            State {name: "SHOW"},
                            State {name: "HIDE"}
                        ]

                        transitions: [
                            Transition {
                                from: "SHOW"
                                to: "HIDE"
                                SequentialAnimation {
                                    NumberAnimation {
                                        target: showRec
                                        property: "height"
                                        from: 200
                                        to: 0
                                        duration: 200
                                    }
                                    PropertyAction {
                                        target: showRec
                                        property: "visible"
                                        value: false
                                    }
                                }
                            },
                            Transition {
                                from: "HIDE"
                                to: "SHOW"
                                SequentialAnimation {
                                    PropertyAction {
                                        target: showRec
                                        property: "visible"
                                        value: true
                                    }
                                    NumberAnimation {
                                        target: showRec
                                        property: "height"
                                        from: 0
                                        to: 200
                                        duration: 200
                                    }
                                }
                            }
                        ]
                        Layout.preferredHeight: 200
                        Layout.fillWidth: true
                        color: "yellow"
                        radius: 8
                        layer.enabled: true
                        layer.effect: DropShadow {
                            radius: 16
                            samples: 16
                            color: "#24222D37"
                            horizontalOffset: 0
                            verticalOffset: 4
                        }
                    }
                    Rectangle {
                        Layout.preferredHeight: 212
                        Layout.fillWidth: true
                        color: "#FDFDFE"
                        radius: 8
                        layer.enabled: true
                        layer.effect: DropShadow {
                            radius: 16
                            samples: 16
                            color: "#24222D37"
                            horizontalOffset: 0
                            verticalOffset: 4
                        }
                    }
                }
            }
        }
    }

    background: Rectangle {
        anchors.fill: parent
        border.width: 1
        border.color: "#404347"
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#66F0F3F7"
            }
            GradientStop {
                position: 1.0
                color: "#FFF2F6FA"
            }
        }
    }
}
