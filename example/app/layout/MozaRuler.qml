import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.12
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: control
    property int rulerWidth: 24
    property int scaleWidth: 1
    property int scaleHeight: 8
    property color highlightColor: "#F66400"
    property color defaultColor: "#686868"
    property color backgroundColor: "#38393C"

    property int scalePercentage: 100
    // deltaLength = 屏幕像素 / 实际长度
    readonly property real deltaLength: control.scalePercentage / 100
    property int scaleMinPercentage: 10
    property int scaleMaxPercentage: 1000
    property int scaleDeltaPercentage: 15
    property int intervalAsymptote: 60
    property var intervalNumber: [1, 2, 5, 10, 25, 50, 100, 250, 500, 1000]

    property real xBegin: -100
    property real yBegin: -100
    property bool highlight: false
    property real xHighlightBegin: 275
    property real xHighlightEnd: 434
    property real yHighlightBegin: 200
    property real yHighlightEnd: 400

    // 恢复默认值比例
    signal restore()

    Rectangle {
        id: leftRuler
        anchors.top: control.top
        anchors.bottom: control.bottom
        anchors.left: control.left
        anchors.right: leftLine.left
        color: control.backgroundColor
        ColumnLayout {
            y: _p.yBeginOffset
            spacing: _p.interval * control.deltaLength
            anchors.right: parent.right
            Repeater {
                model: _p.yScaleNumber
                Item {
                    width: control.rulerWidth
                    height: 0
                    Rectangle {
                        id: leftScaleLine
                        width: control.scaleHeight
                        height: control.scaleWidth
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        color: control.defaultColor
                    }
                    Text {
                        anchors.verticalCenter: leftScaleLine.verticalCenter
                        anchors.right: leftScaleLine.left
                        anchors.rightMargin: 2
                        rotation: -90
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "PingFangSC-Regular"
                        text: index * _p.interval + _p.yBeginScale
                        color: control.defaultColor
                        font.pixelSize: 8
                    }
                }
            }
        }
    }

    Rectangle {
        id: topRuler
        anchors.top: control.top
        anchors.bottom: topLine.top
        anchors.left: control.left
        anchors.right: control.right
        color: control.backgroundColor
        RowLayout {
            x: _p.xBeginOffset
            spacing: _p.interval * control.deltaLength
            anchors.bottom: parent.bottom
            Repeater {
                model: _p.xScaleNumber
                Item {
                    width: 0
                    height: control.rulerWidth
                    Rectangle {
                        id: topScaleLine
                        width: control.scaleWidth
                        height: control.scaleHeight
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: control.defaultColor
                    }
                    Text {
                        anchors.horizontalCenter: topScaleLine.horizontalCenter
                        anchors.bottom: topScaleLine.top
                        anchors.bottomMargin: 2
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "PingFangSC-Regular"
                        text: index * _p.interval + _p.xBeginScale
                        color: control.defaultColor
                        font.pixelSize: 8
                    }
                }
            }
        }
    }

    Rectangle {
        id: topLine
        y: control.rulerWidth
        height: 1
        width: control.width
        color: "black"
    }

    Rectangle {
        id: leftLine
        x: control.rulerWidth
        height: control.height
        width: 1
        color: "black"
    }

    Rectangle {
        id: restore_icon
        width: control.rulerWidth
        height: control.rulerWidth
        anchors.left: control.left
        anchors.top: control.top
        color: control.backgroundColor
        ColorImage {
            anchors.centerIn: parent
            source: "qrc:/icons/scaleplate_adaptation_normal.svg"
            sourceSize: Qt.size(control.rulerWidth - 2, control.rulerWidth - 2)
            TapHandler {
                onSingleTapped: control.restore()
            }
        }
    }

    Rectangle {
        y: Math.floor((control.yHighlightBegin - control.yBegin) * control.deltaLength)
        width: 1
        height: (control.yHighlightEnd - control.yHighlightBegin) * control.deltaLength
        anchors.horizontalCenter: leftLine.horizontalCenter
        visible: control.highlight
        color: control.highlightColor
        Rectangle {
            width: control.rulerWidth
            height: parent.height
            anchors.right: parent.left
            opacity: 0.2
            color: control.highlightColor
        }
    }

    Rectangle {
        x: Math.floor((control.xHighlightBegin - control.xBegin) * control.deltaLength)
        width: (control.xHighlightEnd - control.xHighlightBegin) * control.deltaLength
        height: 1
        anchors.verticalCenter: topLine.verticalCenter
        visible: control.highlight
        color: control.highlightColor
        Rectangle {
            width: parent.width
            height: control.rulerWidth
            anchors.bottom: parent.top
            opacity: 0.2
            color: control.highlightColor
        }
    }

    QtObject {
        id: _p
        property real xEnd: xBegin + control.width / deltaLength
        property real yEnd: yBegin + control.height / deltaLength
        property int interval: {
            let flag = intervalNumber.length - 1
            for (let i = 0; i < intervalNumber.length; ++i) {
                let cur = intervalNumber[i] * deltaLength
                if (cur > intervalAsymptote) {
                    flag = i
                    break
                }
            }
            if (flag == 0) return intervalNumber[flag]
            let curDiv = Math.abs(intervalNumber[flag] * deltaLength - intervalAsymptote)
            let prevDiv = Math.abs(intervalNumber[flag - 1] * deltaLength - intervalAsymptote)

            return prevDiv < curDiv ? intervalNumber[flag - 1] : intervalNumber[flag]
        }
        property int xScaleNumber: Math.floor(xEnd / interval) - Math.floor(xBegin / interval)
        property int xBeginScale: Math.floor(xBegin / interval + 1) * interval
        property real xBeginOffset: (xBeginScale - xBegin) * deltaLength
        property int yScaleNumber: Math.floor(yEnd / interval) - Math.floor(yBegin / interval)
        property int yBeginScale: Math.floor(yBegin / interval + 1) * interval
        property real yBeginOffset: (yBeginScale - yBegin) * deltaLength
    }

    function increaseScale(xAxis, yAxis) {
        let xPos = xBegin + xAxis / control.deltaLength, yPos = yBegin + yAxis / control.deltaLength
        if (control.scalePercentage < control.scaleMaxPercentage - control.scaleDeltaPercentage) {
            control.scalePercentage += control.scaleDeltaPercentage
        } else {
            control.scalePercentage = control.scaleMaxPercentage
        }
        xBegin = xPos - xAxis / control.deltaLength
        yBegin = yPos - yAxis / control.deltaLength
    }
    function decreaseScale(xAxis, yAxis) {
        let xPos = xBegin + xAxis / control.deltaLength, yPos = yBegin + yAxis / control.deltaLength
        if (control.scalePercentage > control.scaleMinPercentage + control.scaleDeltaPercentage) {
            control.scalePercentage -= control.scaleDeltaPercentage
        } else {
            control.scalePercentage = control.scaleMinPercentage
        }
        xBegin = xPos - xAxis / control.deltaLength
        yBegin = yPos - yAxis / control.deltaLength
    }
}
