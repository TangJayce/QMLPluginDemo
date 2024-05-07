import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "../layout"
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
                    text: "切换标尺"
                    onClicked: {
                        artBoard.enableRuler = !artBoard.enableRuler
                    }
                }
                MozaButton {
                    text: "文本"
                    onClicked: {
                        screenModel.move(1, 2, 1)
                    }
                }
                MozaButton {
                    text: "图片"
                }
                MozaButton {
                    text: "椭圆"
                }
                MozaButton {
                    text: "方框"
                }
                MozaButton {
                    text: "预绑定遥测"
                }
                MozaButton {
                    text: "地图"
                }
                MozaButton {
                    text: "刻度仪表"
                }
            }

            RowLayout {
                ColumnLayout {
                    Repeater {
                        model: ListModel {
                            id: screenModel
                            ListElement {name: "screen 233"}
                            ListElement {name: "jayce"}
                            ListElement {name: "adil"}
                        }
                        delegate: Rectangle {
                            Layout.preferredWidth: 260
                            Layout.preferredHeight: 20
                            color: '#2D2F31'
                            opacity: 0.8
                            radius: 4
                            Text {
                                anchors.centerIn: parent
                                text: model.name
                                color: "#DADADA"
                            }
                            Component.onCompleted: {
                                console.log("新建", model.name)
                            }
                            Component.onDestruction: {
                                console.log("销毁", model.name)
                            }
                        }
                    }
                    ContentLayer {
                        id: mozaTreeView
                        Layout.preferredWidth: 260
                        Layout.fillHeight: true
                        tree: _p.tree
                        interactor: _p.interactor
                    }
                }

                ArtBoard {
                    id: artBoard
                    Layout.fillWidth: true
                    Layout.preferredHeight: 600
                    tree: _p.tree
                    interactor: _p.interactor
                }

                Rectangle {
                    Layout.preferredWidth: 302
                    Layout.fillHeight: true
                    color: "#2D2F31"
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

    QtObject {
        id: _p
        property ElementTree tree: DashboardEditorManager.parseJsonFile("D:/MyFiles/test/editor.json")
        property ElementInteractor interactor: ElementInteractor {
            tree: _p.tree
            store: DashboardEditorManager.getDashboardStore()
        }
    }

    focus: true
    Keys.forwardTo: [artBoard]
}
