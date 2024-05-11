import QtQuick 2.15

import MOZA.DashboardEditorModel 1.0

Item {
    id: control

    property int elementID: -1
    property var _InternalProps: DashboardEditorManager.getDashboardStore().getInternalProperties(control.elementID)
    property var _CustomProps: DashboardEditorManager.getDashboardStore().getCustomProperties(control.elementID)
    property real arc: _InternalProps.angle * Math.PI / 180
    property matrix4x4 matrix: Qt.matrix4x4(Math.cos(arc), -Math.sin(arc), 0, _InternalProps.xOffset+width/2*(1-Math.cos(arc))+height/2*Math.sin(arc),
                                            Math.sin(arc),  Math.cos(arc), 0, _InternalProps.yOffset+height/2*(1-Math.cos(arc))-width/2*Math.sin(arc),
                                                        0,              0, 1,                                                                       0,
                                                        0,              0, 0,                                                                       1)
    width: _InternalProps.width
    height:  _InternalProps.height
    visible: !(_CustomProps.hidden || _CustomProps.parentHidden)
}
