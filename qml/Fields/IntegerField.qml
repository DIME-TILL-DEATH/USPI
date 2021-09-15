import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0

Rectangle{
    id: _root

    property var adapter

    // property не обновляется само при изменении в адаптере
    property int value: (adapter !== undefined) ? adapter.value : 0
    property int valueFrom : (adapter !== undefined) ? adapter.valueFrom : 0
    property int valueTo: (adapter !== undefined) ? adapter.valueTo : 255

    signal fieldChanged(fieldId : string, newValue : int)

    width: parent.width
    height: parent.height/Style.fieldsOnScreen
    border.width: 1
    radius: height/5

    FieldTip{
        tipText: (adapter !== undefined) ? adapter.name + ":\n\n" + "min: " + adapter.valueFrom + "\nmax: " +adapter.valueTo + "\n" + adapter.description
                                         : "описание"
    }

    Row{
        width: parent.width
        height: parent.height

        spacing: width / 50

        FieldHeader{
            adapter: _root.adapter
        }

        TextField{
            id: _textField1

            height: parent.height*0.75
            anchors.verticalCenter: parent.verticalCenter

            text: value

            selectByMouse: true
            inputMethodHints: Qt.ImhDigitsOnly


//            validator: IntValidator{
//                bottom: _root.valueFrom
//                top: _root.valueTo
//            }

            background: Rectangle {
                     implicitWidth: _textField1.font.pointSize * 10
                     implicitHeight: _textField1.font.pointSize

                     color: (parseInt(_textField1.text) < _root.valueFrom) |
                            (parseInt(_textField1.text) > _root.valueTo) ?
                                "red" : "transparent"

                     border.color: _textField1.activeFocus ? Style.borderColorActive : Style.borderColorPassive
                 }


            onEditingFinished: {
                //adapter.value = parseInt(text)

                adapter.value = text

                _textField1.focus = false
                fieldChanged(adapter.name, parseInt(text))

                _textField2.text = (adapter.value * adapter.fieldScale.coefficient + adapter.fieldScale.offset).toFixed(2)
            }

            onTextEdited: {
                _textField2.text = (parseFloat(_textField1.text) * adapter.fieldScale.coefficient + adapter.fieldScale.offset).toFixed(2)
            }
        }


        Text{
            anchors.verticalCenter: parent.verticalCenter

            text: "="
            visible: (adapter !== undefined) ? (adapter.fieldScale.coefficient !== 1) : false
        }

        Text{
            id: _textField2

            anchors.verticalCenter: parent.verticalCenter

            text: (adapter !== undefined) ? (adapter.value * adapter.fieldScale.coefficient + adapter.fieldScale.offset).toFixed(2) : 0
            visible: (adapter !== undefined) ? (adapter.fieldScale.coefficient !== 1) : false
        }

        Text{
            anchors.verticalCenter: parent.verticalCenter

            text: (adapter !== undefined) ? adapter.fieldScale.units : ""
            visible: (adapter !== undefined) ? (adapter.fieldScale.coefficient !== 1) : false
        }
    }
}
