import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

Rectangle{
    id: _root

    property var adapter

    property int value: (adapter !== undefined) ? adapter.value : 0
    property int valueFrom : (adapter !== undefined) ? adapter.valueFrom : 0
    property int valueTo: (adapter !== undefined) ? adapter.valueTo : 255

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

        TextField{
            id: _textField

            height: parent.height*0.75
            anchors.verticalCenter: parent.verticalCenter

            text: value

            selectByMouse: true
            inputMethodHints: Qt.ImhDigitsOnly

            validator: IntValidator{
                bottom: _root.valueFrom
                top: _root.valueTo
            }

            background: Rectangle {
                     implicitWidth: _textField.font.pointSize * 10
                     implicitHeight: _textField.font.pointSize

                     color: (parseInt(_textField.text) < _root.valueFrom) |
                            (parseInt(_textField.text) > _root.valueTo) ?
                                "red" : "transparent"

                     border.color: _textField.activeFocus ? "blue" : "gray"
                 }


            onEditingFinished: {
                adapter.value = parseInt(text)
                _textField.focus = false
                fieldChanged(adapter.name, parseInt(text))
            }
        }
    }
}
