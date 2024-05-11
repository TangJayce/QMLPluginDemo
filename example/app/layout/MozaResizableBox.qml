import QtQuick 2.12

Rectangle {
    id: control

    signal resizeInLeftTopArea(real w, real h)
    signal resizeInRightTopArea(real w, real h)
    signal resizeInLeftBottomArea(real w, real h)
    signal resizeInRightBottomArea(real w, real h)

    property bool resizing: false
    property bool enableResize: true
    property int cornerSize: 8
    property real minWidth: 2
    property real minHeight: 2
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
        visible: control.enableResize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPressed: control.resizing = true
            onReleased: control.resizing = false
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(rightBottomArea, mouse.x, mouse.y)
                    resizeInLeftTopArea(Math.max(deltaSize - p.x, minWidth), Math.max(deltaSize - p.y, minHeight))
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
        visible: control.enableResize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPressed: control.resizing = true
            onReleased: control.resizing = false
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(leftBottomArea, mouse.x, mouse.y)
                    resizeInRightTopArea(Math.max(p.x - deltaSize, minWidth), Math.max(deltaSize - p.y, minHeight))
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
        visible: control.enableResize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPressed: control.resizing = true
            onReleased: control.resizing = false
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(rightTopArea, mouse.x, mouse.y)
                    resizeInLeftBottomArea(Math.max(deltaSize - p.x, minWidth), Math.max(p.y - deltaSize, minHeight))
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
        visible: control.enableResize
        color: control.cornerColor
        MouseArea {
            width: parent.width + 4
            height: parent.width + 4
            anchors.centerIn: parent
            cursorShape: Qt.SizeAllCursor
            onPressed: control.resizing = true
            onReleased: control.resizing = false
            onPositionChanged: {
                if (pressed) {
                    let p = mapToItem(leftTopArea, mouse.x, mouse.y)
                    resizeInRightBottomArea(Math.max(p.x - deltaSize, minWidth), Math.max(p.y - control.deltaSize, minHeight))
                }
            }
        }
    }
}
