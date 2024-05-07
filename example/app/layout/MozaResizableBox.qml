import QtQuick 2.12

Rectangle {
    id: control

    signal resizeInLeftTopArea(real w, real h)
    signal resizeInRightTopArea(real w, real h)
    signal resizeInLeftBottomArea(real w, real h)
    signal resizeInRightBottomArea(real w, real h)

    property int cornerSize: 8
    property color cornerColor: "white"
    readonly property real deltaSize: (cornerSize - border.width) / 2

    color: "transparent"
    border.width: 2

    Rectangle {
        id: leftTopArea
        width: control.cornerSize
        height: control.cornerSize
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: -control.deltaSize
        anchors.topMargin: -control.deltaSize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(rightBottomArea, mouse.x, mouse.y)
                    control.resizeInLeftTopArea(Math.max(control.deltaSize - p.x, 0), Math.max(control.deltaSize - p.y, 0))
                }
            }
        }
    }
    Rectangle {
        id: rightTopArea
        width: control.cornerSize
        height: control.cornerSize
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: -control.deltaSize
        anchors.topMargin: -control.deltaSize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(leftBottomArea, mouse.x, mouse.y)
                    control.resizeInRightTopArea(Math.max(p.x - control.deltaSize, 0), Math.max(control.deltaSize - p.y, 0))
                }
            }
        }
    }
    Rectangle {
        id: leftBottomArea
        width: control.cornerSize
        height: control.cornerSize
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: -control.deltaSize
        anchors.bottomMargin: -control.deltaSize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(rightTopArea, mouse.x, mouse.y)
                    control.resizeInLeftBottomArea(Math.max(control.deltaSize - p.x, 0), Math.max(p.y - control.deltaSize, 0))
                }
            }
        }
    }
    Rectangle {
        id: rightBottomArea
        width: control.cornerSize
        height: control.cornerSize
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: -control.deltaSize
        anchors.bottomMargin: -control.deltaSize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(leftTopArea, mouse.x, mouse.y)
                    control.resizeInRightBottomArea(Math.max(p.x - control.deltaSize, 0), Math.max(p.y - control.deltaSize, 0))
                }
            }
        }
    }
}
