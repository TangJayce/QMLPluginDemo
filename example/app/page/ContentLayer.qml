import QtQuick 2.15

import MOZA.DashboardEditor 1.0
import "../layout"
import "../component"

MozaTreeView {
    id: root
    required property ElementInteractor interactor
    property bool enableHide: true
    property bool enableLock: true

    // highlight: Rectangle {
    //     color: "yellow"
    //     anchors.right: parent.right
    //     height: 2
    //     width: root.availableWidth - root.treeModel.itemIndexDepth(root.treeModel.frameIndex) * 8
    //     y: _p.highlightYAxis
    //     z: 100
    //     visible: _p.hasItemDrag
    // }
    delegate: MozaContentLayerItem {
        id: delegateItem
        property var props: DashboardEditorManager.getDashboardStore().getProperties(model.elementID)
        text: "value: " + model.elementID + ", depth: " + model.depth
        width: root.availableWidth
        enableHide: root.enableHide
        enableLock: root.enableLock
        // hideStatus: model.hideStatus
//        lockStatus: props.selected
        selectStatus: {
            if (props.selected) return 1
            if (props.parentSelected) return 2
            return 0
        }
        // onToggleHide: root.treeModel.toggleHide(model.index)
//        onToggleLock: props.selected = !props.selected
        // highlighted: model.index === root.treeModel.frameIndex
        // onHoveredChanged: {
        //     if (hovered && !_p.hasItemDrag) {
        //         root.treeModel.frameIndex = model.index
        //     }
        // }
        onIsDraggingChanged: {
            _p.hasItemDrag = isDragging
            // if (!isDragging) {
            //     root.treeModel.moveSingleItem(model.index, _p.gapIndex)
            // }
        }
        onDragY: {
            let topY = y + yAxis
            let total = root.treeModel.rowCount()
            let cnt = 0
            if (topY > delegateItem.height * total) {
                cnt = total
            } else if (topY > 0) {
                cnt = Math.round(topY / delegateItem.height)
            }
            // root.treeModel.frameIndex = root.treeModel.dragGapIndex(cnt)
            _p.gapIndex = cnt
            _p.highlightYAxis = cnt * delegateItem.height
        }
         onSwitchSelectItem: root.interactor.selectedElements = [model.elementID]
        // onAttachSelectItem: root.treeModel.attachSelect(model.index)
        // onMultipleSelectItem: root.treeModel.multipleSelect(ListView.view.currentIndex, model.index)
        onToggleExpand: {
            if (model.expanded) {
                root.treeModel.collapseRow(index)
                console.log("折叠" + index)
            } else {
                root.treeModel.expandRow(index)
                console.log("展开" + index)
            }
        }
        onEditName: {
            console.log("编辑名称:", index)
        }
        onOpenMenu: {
            console.log("打开菜单:", xAxis, y + yAxis)
        }
    }
    QtObject {
        id: _p
        property int highlightYAxis: 0
        property int gapIndex: -1
        property bool hasItemDrag: false
    }
}
