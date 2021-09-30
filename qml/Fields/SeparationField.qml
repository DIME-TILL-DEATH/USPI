import QtQuick 2.0
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

Rectangle{
    id: _root

    property var adapter
    //property int value: (adapter !== undefined) ? adapter.value : 0

    property int fieldIndex : 0

    signal fieldChanged(fieldId : string, newValue : int)

    width: parent.width
    height: parent.height/Style.fieldsOnScreen
    border.width: 1
    radius: height/5

    color: Style.separationField

    FieldTip{
        tipText: (adapter !== undefined) ? (adapter.name + ":\n\n" + adapter.description) : "описание"
    }

//    Row{
//        width: parent.width
//        height: parent.height

//        spacing: width / 50

        Text{
            id: _fieldName

            anchors{
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            width: _root.width-parent.padding*2

            text: (adapter !== undefined) ? adapter.name : "Имя поля"
            elide: Text.ElideMiddle
        }
//    }
}
