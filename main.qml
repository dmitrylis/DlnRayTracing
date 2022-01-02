import QtQuick 2.15
import QtQuick.Window 2.15

import com.dln.rendering 1.0

Window {
    width: 1024
    height: 768
    visible: true
    title: qsTr("Ray Tracing")

    DlnView {
        anchors.fill: parent
    }
}
