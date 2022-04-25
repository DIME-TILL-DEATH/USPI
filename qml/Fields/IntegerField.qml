import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Qt.labs.platform 1.1

import StyleSettings 1.0
import Elements 1.0

Rectangle{
    id: _root

    property var adapter

    // property не обновляется само при изменении в адаптере
    property var value: (adapter !== undefined) ? adapter.value : 0
    property int valueFrom : (adapter !== undefined) ? adapter.valueFrom : 0
    property int valueTo: (adapter !== undefined) ? adapter.valueTo : 255

    signal fieldChanged(fieldId : string, newValue : int)


    width: parent.width
    height: parent.height/Style.fieldsOnScreen
    border.width: 1
    radius: height/5

    FieldTip{
        tipText: (adapter !== undefined) ? adapter.name + ":\n\n" + "min: "
                                            + adapter.valueFrom + "\nmax: "
                                            + adapter.valueTo + "\n"
                                            + adapter.description
                                            + ((adapter.comment!=="") ? "\n\n" + adapter.comment : "")
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

            background: Rectangle {
                     id: _backgroundRectangle

                     implicitWidth: _textField1.font.pointSize * 15
                     implicitHeight: _textField1.font.pointSize

                     color: "transparent"

                     border.color: _textField1.activeFocus ? Style.borderColorActive : Style.borderColorPassive
                 }


            onEditingFinished: {
                _textField1.focus = false
                _textField1.text = adapter.value
                _textField2.text = adapter.scaledValue
            }

            onTextEdited: {
                adapter.value = text

                _backgroundRectangle.color = (_root.adapter.isValidValue) ? "transparent" : "red"
                fieldChanged(adapter.name, parseInt(text))

                _textField2.text = adapter.scaledValue
            }
        }


        Text{
            anchors.verticalCenter: parent.verticalCenter

            text: "="
            visible: (adapter !== undefined) ? (adapter.scaledValue !== "none") : false
        }

        TextField{
            id: _textField2

            height: parent.height*0.75
            anchors.verticalCenter: parent.verticalCenter

            selectByMouse: true
            inputMethodHints: Qt.ImhDigitsOnly

            background: Rectangle {
                     implicitWidth: _textField2.font.pointSize * 15
                     implicitHeight: _textField2.font.pointSize

                     color: "transparent"

                     border.color: _textField2.activeFocus ? Style.borderColorActive : Style.borderColorPassive
            }

            text: (adapter !== undefined) ? adapter.scaledValue : 0
            visible: (adapter !== undefined) ? (adapter.scaledValue !== "none") : false

            onEditingFinished: {
                adapter.scaledValue = text

                _textField2.focus = false

                _textField1.text = adapter.value
                _textField2.text = adapter.scaledValue
            }

            onTextEdited: {
                adapter.scaledValue = text

                _backgroundRectangle.color = (_root.adapter.isValidValue) ? "transparent" : "red"
                fieldChanged(adapter.name, parseInt(text))
            }
        }

        Text{
            anchors.verticalCenter: parent.verticalCenter

            text: (adapter !== undefined) ? "" : ""
            visible: (adapter !== undefined) ? (adapter.scaledValue !== "none") : false
        }
    }

    //[for menu]
    MouseArea{
        id: _ma

        z: - 10

        anchors.fill: parent

        acceptedButtons: Qt.AllButtons

        onClicked: {
            if(mouse.button & Qt.RightButton)
            {
                _viewOptionsMenu.open()
            }
        }
    }

    Menu{
        id: _viewOptionsMenu

        MenuItemGroup
        {
            id: _itemGrop
        }

        MenuItem{
            text: "BIN"
            checkable: true
            group: _itemGrop
            onTriggered: {
                adapter.viewOptions = 2
                _textField1.text = adapter.value
            }
        }
        MenuItem{
            text: "DEC"
            checkable: true
            checked: true
            group: _itemGrop
            onTriggered: {
                adapter.viewOptions = 10
                _textField1.text = adapter.value
            }
        }

        MenuItem{
            text: "HEX"
            checkable: true
            group: _itemGrop
            onTriggered: {
                adapter.viewOptions = 16
                _textField1.text = adapter.value
            }
        }
    }

}
