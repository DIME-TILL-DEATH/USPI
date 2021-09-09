import QtQuick 2.15
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent

    property string tipText : ""

    ToolTip{
        id: _toolTip

        delay: 1000
        timeout: 10000
        visible: _mouseArea.containsMouse

        contentItem: Text{
            text: tipText

            //elide: Text.ElideRight
           // wrapMode: Text.WordWrap
        }
    }

    MouseArea{
        id: _mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
