import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.12

ItemDelegate {
    id: control

    property bool hasTopRadius: true
    property bool hasBottomRadius: true
    property real backgroundOpacity: 1
    property bool isFrameItem: false
    property alias isDragging: dragHandler.active

    signal toggleExpand()
    signal switchSelectItem()
    signal attachSelectItem()
    signal multipleSelectItem()
    signal dragY(int yAxis)

    padding: 6
    leftPadding: model.depth * 8

    checkable: true
    font.family: "Source Han Sans SC"
    font.pixelSize: 12
    icon.color: control.checked ? "#FFFFFF" : "#B3B3B3"
    icon.width: 12
    icon.height: 12
    icon.source: "qrc:/icons/down_normal"

    contentItem: Text {
        leftPadding: control.indicator.width + control.spacing
        text: control.text
        font: control.font
        color: control.checked ? "#FFFFFF" : "#DADADA"
        opacity: control.enabled ? 1 : 0.3
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    indicator: ColorImage {
        visible: model.hasChild
        source: icon.source
        sourceSize: Qt.size(control.icon.width, control.icon.height)
        color: control.icon.color
        x: control.leftPadding
        y: control.height / 2 - height / 2
        rotation: expanded ? 0 : -90
        Behavior on rotation {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
        Item {
            width: parent.width + 4
            height: parent.height + 4
            anchors.centerIn: parent
            TapHandler {
                onTapped: control.toggleExpand()
            }
        }
    }

    background: Rectangle {
        implicitHeight: 28
        implicitWidth: 228
        color: "transparent"
        border.width: control.highlighted ? 1 : 0
        border.color: "#F66400"
        radius: 4
        RadiusRectangle {
            anchors.fill: parent
            radius: 4
            opacity: control.backgroundOpacity
            color: control.checked ? "#F66400" : "transparent"
            hasLeftTop: control.hasTopRadius
            hasRightTop: control.hasTopRadius
            hasLeftBottom: control.hasBottomRadius
            hasRightBottom: control.hasBottomRadius
        }
        DragHandler {
            id: dragHandler
            target: copyImage
            xAxis.enabled: false
            onActiveChanged: {
                if (active) {
                    control.switchSelectItem()
                    control.grabToImage(function(result){
                        copyImage.source = result.url
                    })
                } else {
                    copyImage.source = ""
                }
            }
        }
    }
    TapHandler {
        onSingleTapped: {
            if (point.modifiers === Qt.ControlModifier) {
                control.attachSelectItem()
            } else if (point.modifiers === Qt.ShiftModifier) {
                control.multipleSelectItem()
            } else {
                control.switchSelectItem()
            }
            control.ListView.view.currentIndex = model.index
        }

        onDoubleTapped: {
            // todo: 编辑名称
            if (point.modifiers === Qt.ControlModifier) { return }
            console.log("编辑名称")
        }
    }
    Image {
        id: copyImage
        width: control.width
        height: control.height
        opacity: 0.3
        onYChanged: {
            if (dragHandler.active) {
                control.dragY(y)
            }
        }
    }
}
