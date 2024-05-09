import QtQuick 2.15
import QtQuick.Shapes 1.15

MozaDashboardElement {
    id: control
    property int borderWidth: 2

    Shape {
        width: control.width
        height: control.height
        antialiasing: true
        ShapePath {
            strokeWidth: control.borderWidth
            strokeColor: "white"
            fillColor: "lightblue"
            startX: control.width / 2
            startY: 0
            PathAngleArc {
                centerX: control.width / 2
                centerY: control.height / 2
                radiusX: control.width / 2 - control.borderWidth
                radiusY: control.height / 2 - control.borderWidth
                startAngle: 0
                sweepAngle: 360
            }
        }

        Text {
            antialiasing: true
            text: "Ellipse: " + control.elementID
            anchors.centerIn: parent
            font.pixelSize: 20
            color: "red"
        }
    }
}
