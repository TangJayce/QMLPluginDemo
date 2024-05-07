import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.12
import QtQuick.Layouts 1.15

ItemDelegate {
    id: control

    property bool isFrameItem: false
    property alias isDragging: dragHandler.active
    property bool enableHide: true
    property bool enableLock: true
    property int hideStatus: 0
    property int lockStatus: 0
    property int selectStatus: 0
    property bool hasTopRadius: false
    property bool hasBottomRadius: false

    signal toggleExpand()
    signal toggleHide()
    signal toggleLock()
    signal switchSelectItem()
    signal attachSelectItem()
    signal multipleSelectItem()
    signal dragY(real yAxis)
    signal editName()
    signal openMenu(real xAxis, real yAxis)

    spacing: 4
    padding: 0
    leftPadding: model.depth * 8
    rightPadding: 4

    font.family: "Source Han Sans SC"
    font.pixelSize: 12
    icon.color: control.selectStatus !== 0 ? "#FFFFFF" : "#B3B3B3"
    icon.width: 12
    icon.height: 12
    icon.source: "qrc:/icons/icon_text.svg"

    contentItem: RowLayout {
        spacing: control.spacing
        opacity: control.enabled && control.hideStatus === 0 ? 1 : control.selectStatus === 1 ? 0.6 : 0.3
        Item { Layout.preferredWidth: control.indicator.width }
        ColorImage {
            color: control.icon.color
            source: control.icon.source
            sourceSize: Qt.size(20, 20)
        }
        Text {
            Layout.fillWidth: true
            text: control.text
            font: control.font
            color: control.selectStatus !== 0 ? "#FFFFFF" : "#DADADA"
            elide: Text.ElideRight
        }
        ColorImage {
            visible: {
                if (!control.enableLock) return false
                if (control.hovered) return true
                return control.hideStatus !== 0;

            }
            opacity: !control.hovered && control.hideStatus === 0 ? 0 : 1
            color: control.icon.color
            source: {
                if (control.hideStatus === 0) return "qrc:/icons/icon_visible.svg"
                if (control.hideStatus === 1) return "qrc:/icons/icon_invisible.svg"
                return "qrc:/icons/icon_sub_enable.svg"
            }
            sourceSize: Qt.size(20, 20)
            TapHandler {
                onTapped: control.toggleHide()
            }
        }
        ColorImage {
            visible: {
                if (!control.enableLock) return false
                if (control.hovered) return true
                return control.hideStatus !== 0 || control.lockStatus !== 0;

            }
            opacity: !control.hovered && control.lockStatus === 0 ? 0 : 1
            color: control.icon.color
            source: {
                if (control.lockStatus === 0) return "qrc:/icons/icon_unlock.svg"
                if (control.lockStatus === 1) return "qrc:/icons/icon_lock.svg"
                return "qrc:/icons/icon_sub_enable.svg"
            }
            sourceSize: Qt.size(20, 20)
            TapHandler {
                onTapped: control.toggleLock()
            }
        }
    }

    indicator: ColorImage {
        visible: model.hasChild
        source: "qrc:/icons/down_normal.svg"
        sourceSize: Qt.size(control.icon.width, control.icon.height)
        color: control.icon.color
        x: control.leftPadding
        y: control.height / 2 - height / 2
        rotation: model.expanded ? 0 : -90
        Behavior on rotation {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
        MouseArea {
            width: parent.width + 4
            height: parent.height + 4
            anchors.centerIn: parent
            onClicked: {
                control.toggleExpand()
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
        MozaRadiusRectangle {
            anchors.fill: parent
            radius: 4
            opacity: control.selectStatus === 2 ? 0.2 : 1
            color: control.selectStatus !== 0 ? "#F66400" : "transparent"
            hasLeftTop: control.hasTopRadius
            hasRightTop: control.hasTopRadius
            hasLeftBottom: control.hasBottomRadius
            hasRightBottom: control.hasBottomRadius
        }
        DragHandler {
            id: dragHandler
            target: copyImage
            xAxis.enabled: false
            // todo: 拖拽时移动左右来切换深度
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
                control.editName()
            }
        }
        TapHandler {
            acceptedButtons: Qt.RightButton
            onTapped: control.openMenu(eventPoint.position.x, eventPoint.position.y)
        }
    }
}
