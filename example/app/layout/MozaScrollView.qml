import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T

T.ScrollView {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    property real shrinkScrollBar: 0
    property real scrollBarEdgeOffset: 0
    property alias verticalScrollbar: verticalScrollbar
    property alias horizontalScrollbar: horizontalScrollbar

    ScrollBar.vertical: MozaScrollBar {
        id: verticalScrollbar
        parent: control
        x: control.mirrored ? 0 : control.width - width - scrollBarEdgeOffset
        y: control.topPadding + shrinkScrollBar
        height: control.availableHeight - shrinkScrollBar * 2
        active: control.ScrollBar.vertical.active
        policy: ScrollBar.AsNeeded
    }

    ScrollBar.horizontal: MozaScrollBar {
        id: horizontalScrollbar
        parent: control
        x: control.leftPadding + shrinkScrollBar
        y: control.height - height - scrollBarEdgeOffset
        width: control.availableWidth - shrinkScrollBar * 2
        active: control.ScrollBar.horizontal.active
        policy: ScrollBar.AlwaysOff
    }
}
