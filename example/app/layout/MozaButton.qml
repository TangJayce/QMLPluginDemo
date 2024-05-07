import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T
import QtGraphicalEffects 1.15

T.Button {
    id: control

    property bool primary: false

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    padding: 0
    font.bold: true
    focusPolicy: Qt.NoFocus

    QtObject {
        id: config
        property color textColor: primary ? 'white' : "#338BFF"
    }

    contentItem: Label {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        opacity: control.enabled ? 1 : 0.6
        color: {
            if (primary) return config.textColor
            else if (control.pressed) return Qt.darker("#338BFF", 1.2)
            else if (control.hovered) return Qt.lighter("#338BFF", 1.2)
            else return config.textColor
        }
        text: control.text
    }

    background: Rectangle {
        id: backgroundRect
        implicitWidth: 90
        implicitHeight: 32

        radius: 4
        opacity: control.enabled ? 1 : 0.4
        border.width: control.primary ? 0 : 2
        border.color: control.pressed || !control.hovered ? "#338BFF" : Qt.lighter("#338BFF", 1.2)
        color: {
            if (!control.primary) {
                if (control.pressed)
                    return "#26338BFF"
                else
                    return "transparent"
            } else
                return "#338BFF"
        }
        layer.enabled: control.primary
        layer.effect: LinearGradient {
            start: Qt.point(0, 0)
            end: Qt.point(width, 0)
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: control.pressed ? "#338BFF" : Qt.lighter("#338BFF", 1.2)
                }
                GradientStop {
                    position: 1.0
                    color: control.pressed || !control.hovered ? "#338BFF" : Qt.lighter("#338BFF", 1.2)
                }
            }
        }
    }
}
