import QtQuick 2.15
import QtQuick.Controls 2.15

import MOZA.DashboardEditor 1.0
import "../layout"

MozaScrollView {
    id: control
    required property ElementTree tree
    property alias delegate: contentListView.delegate
    property alias highlight: contentListView.highlight
    property alias treeModel: contentListView.model

    clip: true
    implicitHeight: 200
    leftPadding: 16
    rightPadding: 16
    topPadding: 8
    bottomPadding: 8

    contentItem: ListView {
        id: contentListView
        model: ElementTreeModel {
            tree: control.tree
        }
        populate: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
        }
        displaced: Transition {
            NumberAnimation { property: "y"; duration: 200 }
        }
        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
        }
        move: Transition {
            NumberAnimation { property: "y"; duration: 200 }
        }
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 200 }
                NumberAnimation { property: "height"; to: 0; duration: 200 }
            }
        }
        highlightFollowsCurrentItem: false
    }
    background: Rectangle {
        color: "#2D2F31"
    }
}
