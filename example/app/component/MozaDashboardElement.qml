import QtQuick 2.15

Item {
    id: control

    property int elementID: -1
    property real rotationNumber: elementID % 12 * 30
    property real xOffset: elementID % 8 * 80
    property real yOffset: (elementID + 4) % 8 * 60
    readonly property real angle: rotationNumber * Math.PI / 180
    property matrix4x4 matrix: Qt.matrix4x4(Math.cos(angle), -Math.sin(angle), 0, xOffset+width/2*(1-Math.cos(angle))+height/2*Math.sin(angle),
                                            Math.sin(angle),  Math.cos(angle), 0, yOffset+height/2*(1-Math.cos(angle))-width/2*Math.sin(angle),
                                                          0,                0, 1,                                                            0,
                                                          0,                0, 0,                                                            1)
    property point p1: Qt.point(xOffset+width/2*(1-Math.cos(angle))+height/2*Math.sin(angle),
                                yOffset+height/2*(1-Math.cos(angle))-width/2*Math.sin(angle))
    property point p2: Qt.point(width*Math.cos(angle)+p1.x, width*Math.sin(angle)+p1.y)
    property point p3: Qt.point(width*Math.cos(angle)-height*Math.sin(angle)+p1.x,
                                width*Math.sin(angle)+height*Math.cos(angle)+p1.y)
    property point p4: Qt.point(-height*Math.sin(angle)+p1.x, height*Math.cos(angle)+p1.y)
    property real xUpper: Math.max(p1.x, p2.x, p3.x, p4.x)
    property real xLower: Math.min(p1.x, p2.x, p3.x, p4.x)
    property real yUpper: Math.max(p1.y, p2.y, p3.y, p4.y)
    property real yLower: Math.min(p1.y, p2.y, p3.y, p4.y)

    width: 160
    height: 100
}
