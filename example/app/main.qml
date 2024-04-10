import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./page"

Window {
    id: mainWindow
    visible: true
    width: 1000
    height: 680
    minimumHeight: 120
    minimumWidth: 360
    flags: Qt.Window | Qt.FramelessWindowHint
    property int bw: 2

    MainPage {
        anchors.fill: parent
    }
}
