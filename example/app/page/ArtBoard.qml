import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Shapes 1.15

import MOZA.DashboardEditor 1.0
import "../layout"

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
                if (!loader || !loader.item) {
                    _p.operatorStatus = ArtBoard.OperatorStatus.Selecting
                    _p.selectedElements = []
                } else {
                    _p.selectedElements = [loader.item]
                    root.interactor.selectedElements = [loader.item.elementID]
                    _p.operatorStatus = ArtBoard.OperatorStatus.MoveElement
                    console.log("select:", loader.item.elementID)
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
                for (let item of _p.selectedElements) {
                    item.xOffset -= deltaX
                    item.yOffset -= deltaY
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
                onLoaded: item.elementID = model.elementID
                source: {
                    let type = DashboardEditorManager.getElementType(model.elementID)
                    switch(type) {
                        case DashboardEditorManager.TEXT: return "../component/TextDashboardElement.qml"
                        case DashboardEditorManager.IMAGE: return "../component/ImageDashboardElement.qml"
                        case DashboardEditorManager.ELLIPSE: return "../component/EllipseDashboardElement.qml"
                        case DashboardEditorManager.BOX: return "../component/BoxDashboardElement.qml"
                        case DashboardEditorManager.TELEMETRY: return "../component/TelemetryDashboardElement.qml"
                        case DashboardEditorManager.MAP: return "../component/MapDashboardElement.qml"
                        case DashboardEditorManager.METER: return "../component/MeterDashboardElement.qml"
                        default: return ""
                    }
                }
                transform: Matrix4x4 {
                    matrix: _p.globalMatrix.times(item.matrix)
                }
            }
        }
    }

    MozaResizableBox {
        id: selectedBox
        x: _p.selectedElements.length === 1 ? (_p.selectedElements[0].xOffset - ruler.xBegin) * ruler.deltaLength : 0
        y: _p.selectedElements.length === 1 ? (_p.selectedElements[0].yOffset - ruler.yBegin) * ruler.deltaLength : 0
        width: _p.selectedElements.length === 1 ? _p.selectedElements[0].width * ruler.deltaLength + root.selectedBoxBorderWidth / 2 : 0
        height: _p.selectedElements.length === 1 ? _p.selectedElements[0].height * ruler.deltaLength + root.selectedBoxBorderWidth / 2 : 0
        visible: _p.selectedElements.length === 1
        rotation: _p.selectedElements.length === 1 ? _p.selectedElements[0].rotationNumber : 0
        border.width: root.selectedBoxBorderWidth
        border.color: root.selectedBoxBorderColor
        antialiasing: true
        onResizeInLeftTopArea: {
            let item = _p.selectedElements[0]
            if (w === item.width && h === item.height) return
            let dw = (item.width - w) / 2, dh = (item.height - h) / 2
            item.xOffset += dw*(1+Math.cos(item.angle)) - dh*Math.sin(item.angle)
            item.yOffset += dh*(1+Math.cos(item.angle)) + dw*Math.sin(item.angle)
            item.width = w
            item.height = h
        }
        onResizeInRightTopArea: {
            let item = _p.selectedElements[0]
            if (w === item.width && h === item.height) return
            let dw = (item.width - w) / 2, dh = (item.height - h) / 2
            item.xOffset += dw*(1-Math.cos(item.angle)) - dh*Math.sin(item.angle)
            item.yOffset += dh*(1+Math.cos(item.angle)) - dw*Math.sin(item.angle)
            item.width = w
            item.height = h
        }
        onResizeInLeftBottomArea: {
            let item = _p.selectedElements[0]
            if (w === item.width && h === item.height) return
            let dw = (item.width - w) / 2, dh = (item.height - h) / 2
            item.xOffset += dw*(1+Math.cos(item.angle)) + dh*Math.sin(item.angle)
            item.yOffset += dh*(1-Math.cos(item.angle)) + dw*Math.sin(item.angle)
            item.width = w
            item.height = h
        }
        onResizeInRightBottomArea: {
            let item = _p.selectedElements[0]
            if (w === item.width && h === item.height) return
            let dw = (item.width - w) / 2, dh = (item.height - h) / 2
            item.xOffset += dw*(1-Math.cos(item.angle)) + dh*Math.sin(item.angle)
            item.yOffset += dh*(1-Math.cos(item.angle)) - dw*Math.sin(item.angle)
            item.width = w
            item.height = h
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
        xBegin: root.screenWidth / 2 - root.width / 2
        yBegin: root.screenHeight / 2 - root.height / 2
        visible: root.enableRuler
        highlight: _p.selectedElements.length === 1
        xHighlightBegin: _p.selectedElements.length === 1 ? _p.selectedElements[0].xLower : 0
        xHighlightEnd: _p.selectedElements.length === 1 ? _p.selectedElements[0].xUpper : 0
        yHighlightBegin: _p.selectedElements.length === 1 ? _p.selectedElements[0].yLower : 0
        yHighlightEnd: _p.selectedElements.length === 1 ? _p.selectedElements[0].yUpper : 0
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
        property int operatorStatus: ArtBoard.OperatorStatus.Normal
        property bool spaceKeyDown: false
        property var selectedElements: []
        readonly property int cursorShape: {
            switch(operatorStatus) {
                case ArtBoard.OperatorStatus.ToMoveArtboard: return Qt.OpenHandCursor
                case ArtBoard.OperatorStatus.MovingArtboard: return Qt.ClosedHandCursor
                default: return Qt.ArrowCursor
            }
        }
        readonly property real deltaAngle: Math.PI / 180
        property matrix4x4 globalMatrix: Qt.matrix4x4(ruler.deltaLength,                 0,  0, -ruler.xBegin*ruler.deltaLength,
                                                      0,                 ruler.deltaLength,  0, -ruler.yBegin*ruler.deltaLength,
                                                      0,                                 0,  1,                               0,
                                                      0,                                 0,  0,                               1)
    }
}
