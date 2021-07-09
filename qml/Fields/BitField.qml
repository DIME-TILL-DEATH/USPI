import QtQuick 2.0
import QtQuick.Controls 2.12

import Elements 1.0

Rectangle{
    id: _root

    property var adapter
    property int value: (adapter !== undefined) ? adapter.value : 0

    property int fieldIndex : 0

    signal fieldChanged(fieldId : string, newValue : int)

    width: parent.width
    height: parent.height/10
    border.width: 1
    radius: height/5

    FieldTip{
        tipText: (adapter !== undefined) ? adapter.description : "описание"
    }

    Row{
        width: parent.width
        height: parent.height

        spacing: width / 50

        FieldProperties{
            adapter: _root.adapter
        }

        CheckBox{
            height: parent.height*0.75
            padding: 0
            spacing: 0
            anchors.verticalCenter: parent.verticalCenter

            checked: value

            onToggled: {
                fieldChanged(adapter.name, checkState)
            }
        }
    }
}
