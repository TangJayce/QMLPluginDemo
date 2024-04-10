import QtQuick 2.15
import QtQuick.Controls 2.15

import MOZA.DashboardEditor 1.0

MozaScrollView {
    id: control
    required property MozaTreeModel treeModel

    clip: true
    implicitHeight: 200
    leftPadding: 8
    rightPadding: verticalScrollbar.visible ? 16 : 8
    topPadding: 8
    bottomPadding: 8

    contentItem: ListView {
        model: control.treeModel
        populate: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
        }
        displaced: Transition {
            NumberAnimation { properties: "y"; duration: 200 }
        }
        add: Transition {
            NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 200 }
        }
        move: Transition {
            NumberAnimation { properties: "y"; duration: 200 }
        }
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 200 }
                NumberAnimation { properties: "height"; to: 0; duration: 200 }
            }
        }
        highlight: Rectangle {
            color: "yellow"//"#F66400"
            anchors.right: parent.right
            height: 2
            width: control.availableWidth - control.treeModel.itemIndexDepth(control.treeModel.frameIndex) * 8
            y: _p.highlightYAxis
            z: 100
            visible: _p.hasItemDrag
        }
        highlightFollowsCurrentItem: false
        delegate: MozaTreeItemDelegate {
            id: delegateItem
            text: "value: " + model.value
            width: control.availableWidth
            checked: model.selectedStatus !== MozaTreeModel.NONE_SELECT
            highlighted: model.index === control.treeModel.frameIndex
            backgroundOpacity: model.selectedStatus === MozaTreeModel.SUB_SELECTED ? 0.2 : 1
            onHoveredChanged: {
                if (hovered && !_p.hasItemDrag) {
                    control.treeModel.frameIndex = model.index
                }
            }
            onIsDraggingChanged: {
                _p.hasItemDrag = isDragging
                if (!isDragging) {
                    control.treeModel.moveSingleItem(model.index, _p.gapIndex)
                }
            }
            onDragY: {
                let topY = y + yAxis
                let total = control.treeModel.rowCount()
                let cnt = 0
                if (topY > delegateItem.height * total) {
                    cnt = total
                } else if (topY > 0) {
                    cnt = Math.round(topY / delegateItem.height)
                }
                control.treeModel.frameIndex = control.treeModel.dragGapIndex(cnt)
                _p.gapIndex = cnt
                _p.highlightYAxis = cnt * delegateItem.height
            }
            onSwitchSelectItem: control.treeModel.switchSelect(model.index)
            onAttachSelectItem: control.treeModel.attachSelect(model.index)
            onMultipleSelectItem: control.treeModel.multipleSelect(ListView.view.currentIndex, model.index)
            onCheckedChanged: {
                if (checked) {
                    ListView.view.adjustRadius(index, true)
                    ListView.view.adjustRadius(index, false)
                }
                ListView.view.adjustRadius(index + 1, true)
                ListView.view.adjustRadius(index - 1, false)
            }
            onToggleExpand: {
                if (model.expanded) {
                    control.treeModel.collapseRow(index)
                    console.log("折叠" + index)
                } else {
                    control.treeModel.expandRow(index)
                    console.log("展开" + index)
                }
            }
        }

        // fixme: 新增或者删除时的圆角效果错误
        function adjustRadius(itemIndex, isTop) {
            let cur = itemAtIndex(itemIndex)
            if (!(cur && cur.checked)) return
            let assignItem = isTop ? itemAtIndex(itemIndex - 1) : itemAtIndex(itemIndex + 1)
            if (isTop) {
                cur.hasTopRadius = !(assignItem && assignItem.checked)
            } else {
                cur.hasBottomRadius = !(assignItem && assignItem.checked)
            }
        }
    }
    background: Rectangle {
        color: "#2D2F31"
    }

    QtObject {
        id: _p
        property int highlightYAxis: 0
        property int gapIndex: -1
        property bool hasItemDrag: false
    }
}
