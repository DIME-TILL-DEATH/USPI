import QtQuick 2.0
import QtQuick.Controls 2.12

import Elements 1.0

Rectangle{
    id: _root

    property var adapter

    signal fieldChanged(fieldId : string, newValue : int)

    width: parent.width
    height: parent.height/10
    border.width: 1
    radius: height/5

    FieldTip{
        tipText: (adapter !== undefined) ? adapter.description : "описание"
//        tipText:  adapter.description
    }

    Row{
        width: parent.width
        height: parent.height

        spacing: width / 50

        FieldProperties{
            adapter: _root.adapter
        }

        ComboBox{
            id: _comboBox

            height: parent.height*0.75
            anchors.verticalCenter: parent.verticalCenter

            model: (adapter !== undefined) ? adapter.variantList : 0


            background: Rectangle {
                     implicitWidth: 120
                     implicitHeight: 40
                     border.color: _comboBox.down ? "blue" : "gray"
                     border.width: _comboBox.visualFocus ? 2 : 1
                     radius: 2
                 }

            currentIndex: (adapter !== undefined) ? find(adapter.value, Qt.MatchFixedString) : 0

            onActivated: {
                _root.adapter.value = textAt(index)
                fieldChanged(adapter.name, textAt(index))
            }

        }
    }
}
