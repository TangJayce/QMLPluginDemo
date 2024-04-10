import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    property bool isMaximized: false
    color: "black"

//    MouseArea{
//        anchors.fill: parent
//        acceptedButtons: Qt.LeftButton //只处理鼠标左键
//        property bool isDoubleClicked: false
//        property point clickPos: Qt.point(0, 0)
//        onPressed: {
//            isDoubleClicked = false;
//            clickPos = Qt.point(mouse.x,mouse.y)
//        }
//        onPositionChanged: {
//            if(!isDoubleClicked && pressed && mainWindow.visibility !== Window.Maximized && mainWindow.visibility !== Window.FullScreen) {
//                var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
//                mainWindow.x += delta.x
//                mainWindow.y += delta.y
//            }
//            if(mainWindow.visibility === Window.Maximized && pressed && !isDoubleClicked) {
//                isMaximized = false
//                mainWindow.showNormal()
//            }
//        }
//        onDoubleClicked: {
//            isDoubleClicked = true; // 这个时候一定不能响应onPositionChanged不然会一直置顶。
//            if (isMaximized) {
//                isMaximized = false
//                mainWindow.showNormal()
//            } else {
//                isMaximized = true
//                mainWindow.showMaximized()
//            }
//        }
//    }
    RowLayout {
        anchors.fill: parent
        spacing: 12
        Item {Layout.preferredWidth: 12}
        Label {
            color: "#FFFFFF"
            opacity: 0.65
            font.family: "Source Han Sans SC"
            font.pixelSize: 14
            font.weight: Font.DemiBold
            text: "Firmware Manager"
        }
        Item {Layout.fillWidth: true}
        Image {
            source: "qrc:/icons/minimize_normal.svg"
            width: 16
            height: 16
            sourceSize: Qt.size(width, height)

            MouseArea {
                width: parent.width + 4
                height: parent.height + 4
                anchors.centerIn: parent
                onClicked: mainWindow.showMinimized()
            }
        }
        Image {
            source: "qrc:/icons/full_screen_normal.svg"
            width: 16
            height: 16
            sourceSize: Qt.size(width, height)

            MouseArea {
                width: parent.width + 4
                height: parent.height + 4
                anchors.centerIn: parent
                onClicked: {
                    if(isMaximized) {
                        isMaximized = false
                        mainWindow.showNormal()
                    } else {
                        isMaximized = true
                        mainWindow.showMaximized()
                    }
                }
            }
        }
        Image {
            source: "qrc:/icons/close_normal.svg"
            width: 16
            height: 16
            sourceSize: Qt.size(width, height)

            MouseArea {
                width: parent.width + 4
                height: parent.height + 4
                anchors.centerIn: parent
                onClicked: mainWindow.close()
            }
        }
        Item {Layout.preferredWidth: 12}
    }
}
