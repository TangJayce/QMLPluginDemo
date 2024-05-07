import QtQuick 2.15

MozaDashboardElement {
    id: control
    Rectangle {
        width: control.width
        height: control.height
        color: "blue"
        antialiasing: true
        Text {
            text: "Image: " + control.elementID
            anchors.centerIn: parent
            font.pixelSize: 20
            color: "yellow"
        }
    }
}
