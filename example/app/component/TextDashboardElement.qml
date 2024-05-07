import QtQuick 2.15

MozaDashboardElement {
    id: control
    rotationNumber: (elementID + 2) * 35
    xOffset: elementID % 12 * 80
    yOffset: elementID % 12 * 60
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
