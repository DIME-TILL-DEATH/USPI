import QtQuick 2.0
import QtQuick.Controls 2.12

import Elements 1.0

Rectangle{
    id: _root

    property int fieldIndex : 0
    property alias properties : _header

    signal fieldChanged(fieldId : int, newValue : int)

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

        TextField{
            height: parent.height*0.75
            anchors.verticalCenter: parent.verticalCenter

            validator: IntValidator{

            }

            inputMethodHints: Qt.ImhDigitsOnly

            onAccepted: {
                fieldChanged(fieldIndex, parseInt(text))
            }
        }
    }
}
