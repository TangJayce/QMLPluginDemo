import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Rectangle {
    height: 212
    width: 900
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