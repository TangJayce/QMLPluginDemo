import QtQuick 2.15
import QtGraphicalEffects 1.15

OpacityMask {
    id: control
    property bool hasLeftTop: false
    property bool hasLeftBottom: false
    property bool hasRightTop: false
    property bool hasRightBottom: false
    property color color: "transparent"
    property int radius: 0

    invert: true

    source: Rectangle {
        width: control.width
        height: control.height
        color: control.color
        opacity: control.opacity
    }

    maskSource: Rectangle {
        width: control.width
        height: control.height
        color: "transparent"
        Rectangle {
            id: corner_mask
            width: control.radius * 2
            height: control.radius * 2
            radius: control.radius
            visible: false
        }
        Rectangle {
            id: corner_rect
            width: corner_mask.width
            height: corner_mask.height
            visible: false
        }
        ShaderEffectSource {
            id: shader_effect_mask
            sourceItem: corner_mask
            sourceRect: Qt.rect(0, 0, corner_mask.radius, corner_mask.radius)
        }
        OpacityMask {
            visible: control.hasLeftTop
            anchors.left: parent.left
            anchors.top: parent.top
            width: corner_mask.radius
            height: corner_mask.radius
            maskSource: shader_effect_mask
            source: corner_rect
            invert: true
        }
        OpacityMask {
            visible: control.hasRightTop
            anchors.right: parent.right
            anchors.top: parent.top
            width: corner_mask.radius
            height: corner_mask.radius
            rotation: 90
            maskSource: shader_effect_mask
            source: corner_rect
            invert: true
        }
        OpacityMask {
            visible: control.hasRightBottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: corner_mask.radius
            height: corner_mask.radius
            rotation: 180
            maskSource: shader_effect_mask
            source: corner_rect
            invert: true
        }
        OpacityMask {
            visible: control.hasLeftBottom
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: corner_mask.radius
            height: corner_mask.radius
            rotation: -90
            maskSource: shader_effect_mask
            source: corner_rect
            invert: true
        }
    }
}
