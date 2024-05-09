import QtQuick 2.15

MozaDashboardElement {
    id: control
    Rectangle {
        width: control.width
        height: control.height
        border.width: 1
        border.color: "white"
        color: "transparent"
        antialiasing: true

        Text {
            text: "测试" + control.elementID
            color: "white"
            font.pixelSize: 24
        }
    }
}
