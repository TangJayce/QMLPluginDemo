import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Shapes 1.15

import MOZA.DashboardEditor 1.0
import "../layout"
import "../component"

Rectangle {
    id: root

    required property ElementTree tree
    required property ElementInteractor interactor
    property bool enableRuler: true
    property int screenWidth: 600
    property int screenHeight: 400
    property int screenRadius: 0
    property color screenColor: "black"
    property color selectedBoxBorderColor: "red"
    property int selectedBoxBorderWidth: 2

    clip: true
    color: "#38393C"

    Keys.onReleased: {
        if (event.isAutoRepeat) return
        if (event.key === Qt.Key_Space) {
            _p.spaceKeyDown = false
            if (!area.pressed) {
                _p.operatorStatus = ArtBoard.OperatorStatus.Normal
            }
        }
    }
    Keys.onSpacePressed: {
        if (event.isAutoRepeat) return
        _p.spaceKeyDown = true
        if (!area.pressed) {
            _p.operatorStatus = ArtBoard.OperatorStatus.ToMoveArtboard
        }
    }

    MouseArea {
        id: area
        //储存鼠标开始时的坐标
        property real startX
        property real startY
        property real stopX
        property real stopY
        anchors.fill: parent
        cursorShape: _p.cursorShape
        onPressed: {
            startX = mouse.x
            startY = mouse.y
            stopX = mouse.x
            stopY = mouse.y
            if ((mouse.button == Qt.LeftButton) && _p.spaceKeyDown) {
                _p.operatorStatus = ArtBoard.OperatorStatus.MovingArtboard
            } else {
                let loader = dashboardElements.childAt(mouse.x, mouse.y)
                if (loader && loader.item && !_p.store.getCustomProperties(loader.item.elementID).locked && !_p.store.getCustomProperties(loader.item.elementID).parentLocked) {
                    root.interactor.switchSelect(loader.item.elementID)
                    _p.operatorStatus = ArtBoard.OperatorStatus.MoveElement
                } else {
                    _p.operatorStatus = ArtBoard.OperatorStatus.Selecting
                    root.interactor.clearSelect()
                }
            }
        }
        onReleased: {
            if (_p.spaceKeyDown) {
                _p.operatorStatus = ArtBoard.OperatorStatus.ToMoveArtboard
            } else {
                _p.operatorStatus = ArtBoard.OperatorStatus.Normal
            }
        }
        onPositionChanged: {
            if (!pressed) return
            // 等比例移动
            let deltaX = (stopX - mouse.x) / ruler.deltaLength
            let deltaY = (stopY - mouse.y) / ruler.deltaLength
            if (_p.operatorStatus === ArtBoard.OperatorStatus.MovingArtboard) {
                ruler.xBegin += deltaX
                ruler.yBegin += deltaY
            } else if (_p.operatorStatus === ArtBoard.OperatorStatus.MoveElement) {
                for (let id of _p.selectedElements) {
                    _p.store.getInternalProperties(id).xOffset -= deltaX
                    _p.store.getInternalProperties(id).yOffset -= deltaY
                }
            }
            stopX = mouse.x
            stopY = mouse.y
        }
        onWheel: {
            if (wheel.modifiers & Qt.ControlModifier) {
                // 放缩
                if (wheel.angleDelta.y > 0) {
                    ruler.increaseScale(wheel.x, wheel.y)
                } else {
                    ruler.decreaseScale(wheel.x, wheel.y)
                }
            } else if (wheel.modifiers & Qt.ShiftModifier) {
                // 左右移动
                let deltaX = 20 / ruler.deltaLength
                if (wheel.angleDelta.y > 0) {
                    ruler.xBegin -= deltaX
                } else {
                    ruler.xBegin += deltaX
                }
            } else {
                // 上下移动
                let deltaY = 20 / ruler.deltaLength
                if (wheel.angleDelta.y > 0) {
                    ruler.yBegin -= deltaY
                } else {
                    ruler.yBegin += deltaY
                }
            }
        }
    }

    Rectangle {
        id: screen
        width: root.screenWidth
        height: root.screenHeight
        color: root.screenColor
        transform: Matrix4x4 {
            matrix: _p.globalMatrix
        }
    }
    Item {
        id: dashboardElements

        Repeater {
            model: ArtBoardListModel {
                tree: root.tree
            }
            Loader {
                transform: Matrix4x4 {
                    matrix: _p.globalMatrix.times(item.matrix)
                }
                Component.onCompleted: {
                    let type = DashboardEditorManager.getElementType(model.elementID)
                    switch(type) {
                        case DashboardEditorManager.TEXT:
                            setSource("../component/TextDashboardElement.qml", {elementID: model.elementID})
                            break
                        case DashboardEditorManager.IMAGE:
                            setSource("../component/ImageDashboardElement.qml", {elementID: model.elementID})
                            break
                        case DashboardEditorManager.ELLIPSE:
                            setSource("../component/EllipseDashboardElement.qml", {elementID: model.elementID})
                            break
                        case DashboardEditorManager.BOX:
                            setSource("../component/BoxDashboardElement.qml", {elementID: model.elementID})
                            break
                        case DashboardEditorManager.TELEMETRY:
                            setSource("../component/TelemetryDashboardElement.qml", {elementID: model.elementID})
                            break
                        case DashboardEditorManager.MAP:
                            setSource("../component/MapDashboardElement.qml", {elementID: model.elementID})
                            break
                        case DashboardEditorManager.METER:
                            setSource("../component/MeterDashboardElement.qml", {elementID: model.elementID})
                            break
                        default:
                            setSource("../component/BoxDashboardElement.qml", {elementID: model.elementID})
                            break
                    }
                }
            }
        }
    }

    MozaResizableBox {
        id: selectedBox
        x: _p.selectedElements.length === 1 ? (_p.store.getInternalProperties(_p.selectedElements[0]).xOffset - ruler.xBegin) * ruler.deltaLength : 0
        y: _p.selectedElements.length === 1 ? (_p.store.getInternalProperties(_p.selectedElements[0]).yOffset - ruler.yBegin) * ruler.deltaLength : 0
        width: _p.selectedElements.length === 1 ? _p.store.getInternalProperties(_p.selectedElements[0]).width * ruler.deltaLength + root.selectedBoxBorderWidth / 2 : 0
        height: _p.selectedElements.length === 1 ? _p.store.getInternalProperties(_p.selectedElements[0]).height * ruler.deltaLength + root.selectedBoxBorderWidth / 2 : 0
        visible: _p.selectedElements.length === 1
        rotation: _p.selectedElements.length === 1 ? _p.store.getInternalProperties(_p.selectedElements[0]).angle : 0
        border.width: root.selectedBoxBorderWidth
        border.color: root.selectedBoxBorderColor
        antialiasing: true
        // todo: 直接修改数据
        onResizeInLeftTopArea: {
            let id = _p.selectedElements[0], aw = w / ruler.deltaLength, ah = h / ruler.deltaLength
            let item = _p.store.getInternalProperties(id)
            if (aw === item.width && ah === item.height) return
            let dw = (item.width - aw) / 2, dh = (item.height - ah) / 2
            item.xOffset += dw*(1+Math.cos(item.angle*Math.PI/180)) - dh*Math.sin(item.angle*Math.PI/180)
            item.yOffset += dh*(1+Math.cos(item.angle*Math.PI/180)) + dw*Math.sin(item.angle*Math.PI/180)
            item.width = aw
            item.height = ah
        }
        onResizeInRightTopArea: {
            let id = _p.selectedElements[0], aw = w / ruler.deltaLength, ah = h / ruler.deltaLength
            let item = _p.store.getInternalProperties(id)
            if (aw === item.width && ah === item.height) return
            let dw = (item.width - aw) / 2, dh = (item.height - ah) / 2
            item.xOffset += dw*(1-Math.cos(item.angle*Math.PI/180)) - dh*Math.sin(item.angle*Math.PI/180)
            item.yOffset += dh*(1+Math.cos(item.angle*Math.PI/180)) - dw*Math.sin(item.angle*Math.PI/180)
            item.width = aw
            item.height = ah
        }
        onResizeInLeftBottomArea: {
            let id = _p.selectedElements[0], aw = w / ruler.deltaLength, ah = h / ruler.deltaLength
            let item = _p.store.getInternalProperties(id)
            if (aw === item.width && ah === item.height) return
            let dw = (item.width - aw) / 2, dh = (item.height - ah) / 2
            item.xOffset += dw*(1+Math.cos(item.angle*Math.PI/180)) + dh*Math.sin(item.angle*Math.PI/180)
            item.yOffset += dh*(1-Math.cos(item.angle*Math.PI/180)) + dw*Math.sin(item.angle*Math.PI/180)
            item.width = aw
            item.height = ah
        }
        onResizeInRightBottomArea: {
            let id = _p.selectedElements[0], aw = w / ruler.deltaLength, ah = h / ruler.deltaLength
            let item = _p.store.getInternalProperties(id)
            if (aw === item.width && ah === item.height) return
            let dw = (item.width - aw) / 2, dh = (item.height - ah) / 2
            item.xOffset += dw*(1-Math.cos(item.angle*Math.PI/180)) + dh*Math.sin(item.angle*Math.PI/180)
            item.yOffset += dh*(1-Math.cos(item.angle*Math.PI/180)) - dw*Math.sin(item.angle*Math.PI/180)
            item.width = aw
            item.height = ah
        }
    }

    Rectangle {
        id: selectingBox
        x: Math.min(area.startX, area.stopX)
        y: Math.min(area.startY, area.stopY)
        width: Math.abs(area.startX - area.stopX)
        height: Math.abs(area.startY - area.stopY)
        visible: _p.operatorStatus === ArtBoard.OperatorStatus.Selecting
        color: "orange"
        opacity: 0.3
    }

    MozaRuler {
        id: ruler
        anchors.fill: root
        visible: root.enableRuler
        xBegin: root.screenWidth / 2 - root.width / 2
        yBegin: root.screenHeight / 2 - root.height / 2
        highlight: _p.selectedElements.length > 0
        xHighlightBegin: root.interactor.xLower
        xHighlightEnd: root.interactor.xUpper
        yHighlightBegin: root.interactor.yLower
        yHighlightEnd: root.interactor.yUpper
        onRestore: {
            ruler.scalePercentage = 100
            ruler.xBegin = root.screenWidth / 2 - root.width / 2
            ruler.yBegin = root.screenHeight / 2 - root.height / 2
        }
    }

    enum OperatorStatus {
        Normal,
        Selecting,
        MoveElement,
        ToMoveArtboard,
        MovingArtboard
    }
    QtObject {
        id: _p
        readonly property DashboardStore store: DashboardEditorManager.getDashboardStore()
        readonly property real deltaAngle: Math.PI / 180
        readonly property int cursorShape: {
            switch(operatorStatus) {
                case ArtBoard.OperatorStatus.ToMoveArtboard: return Qt.OpenHandCursor
                case ArtBoard.OperatorStatus.MovingArtboard: return Qt.ClosedHandCursor
                default: return Qt.ArrowCursor
            }
        }
        property int operatorStatus: ArtBoard.OperatorStatus.Normal
        property bool spaceKeyDown: false
        property var selectedElements: root.interactor.selectedElements
        property matrix4x4 globalMatrix: Qt.matrix4x4(ruler.deltaLength,                 0,  0, -ruler.xBegin*ruler.deltaLength,
                                                      0,                 ruler.deltaLength,  0, -ruler.yBegin*ruler.deltaLength,
                                                      0,                                 0,  1,                               0,
                                                      0,                                 0,  0,                               1)
    }
}
