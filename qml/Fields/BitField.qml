import QtQuick 2.0
import QtQuick.Controls 2.12

import Elements 1.0

Rectangle{
    id: _root

    property alias properties : _header

    width: parent.width
    height: parent.height/10
    border.width: 1
    radius: height/5

    FieldTip{
        tipText: properties.description
    }

    Row{
        width: parent.width
        height: parent.height

        spacing: width / 50

        FieldProperties{
            id: _header
        }

        CheckBox{
            height: parent.height*0.75
            padding: 0
            spacing: 0
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
