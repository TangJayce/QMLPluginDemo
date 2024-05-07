import QtQuick 2.12
import QtQuick.Templates 2.12 as T

T.ScrollBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 0
    visible: {
        if (control.policy === T.ScrollBar.AlwaysOff) return false
        if (control.policy === T.ScrollBar.AlwaysOn) return true
        return control.size < 1.0
    }
    minimumSize: orientation == Qt.Horizontal ? height / width : width / height

    contentItem: Item {
        implicitWidth: 8
        implicitHeight: 8
        Rectangle {
            id: colorRect
            anchors.fill: parent
            radius: 4
            color: {
                if (control.pressed) return Qt.darker("#686868", 1.1)
                else if (control.hovered) return Qt.lighter("#686868", 1.1)
                else return "#686868"
            }
            Behavior on implicitWidth { NumberAnimation { duration: 100 } }
            Behavior on implicitHeight { NumberAnimation { duration: 100 } }
            Behavior on radius { NumberAnimation { duration: 100 } }
        }
    }

//    states: State {
//        name: "active"
//        when: control.policy === T.ScrollBar.AlwaysOn || (control.active && control.size < 1.0)
//    }

//    transitions: [
//        Transition {
//            to: "active"
//            NumberAnimation { targets: [control.contentItem]; property: "opacity"; to: 1.0 }
//        },
//        Transition {
//            from: "active"
//            SequentialAnimation {
//                PropertyAction{ targets: [control.contentItem]; property: "opacity"; value: 1.0 }
//                PauseAnimation { duration: 2450 }
//                NumberAnimation { targets: [control.contentItem]; property: "opacity"; to: 0.0 }
//            }
//        }
//    ]
}
