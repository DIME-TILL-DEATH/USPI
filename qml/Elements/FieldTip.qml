import QtQuick 2.15
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent

    property string tipText : ""

    ToolTip{
        id: _toolTip

        delay: 1000
        timeout: 5000
        visible: _mouseArea.containsMouse
        text: tipText

//        x: _mouseArea.mouseX
//        y: _mouseArea.mouseY
    }

    MouseArea{
        id: _mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
