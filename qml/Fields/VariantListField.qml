import QtQuick 2.12
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

Rectangle{
    id: _root

    property var adapter

    signal fieldChanged(fieldId : string, newValue : int)

    width: parent.width
    height: parent.height/Style.fieldsOnScreen
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
            id: _fieldProperties
            adapter: _root.adapter
        }

        ComboBox{
            id: _comboBox

            height: parent.height*0.75
            width: (parent.width - _fieldProperties.width)*0.9

            anchors.verticalCenter: parent.verticalCenter

            model: (adapter !== undefined) ? adapter.variantList : 0


            background: Rectangle {
                     border.color: _comboBox.down ? Style.borderColorActive : Style.borderColorPassive
                     border.width: _comboBox.visualFocus ? 2 : 1
                     radius: 2
                 }

            currentIndex: (adapter !== undefined) ? find(adapter.value, Qt.MatchFixedString) : 0

            contentItem: Text{
                leftPadding: _comboBox.width/25

                text: _comboBox.displayText
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                maximumLineCount: 1
            }

            delegate: ItemDelegate {
                id: _delegate
                 width: _comboBox.width
                 contentItem: Text {
                     text: modelData
                     font{
                         family: _comboBox.font.family
                         pointSize: _comboBox.font.pointSize
                         bold: (_comboBox.currentIndex==index) ? true : false
                     }
                     verticalAlignment: Text.AlignVCenter

                     elide: Text.ElideRight
                     wrapMode: Text.WordWrap
                 }
                 highlighted: _comboBox.highlightedIndex == index
             }

            onActivated: {
                _root.adapter.value = textAt(index)
                fieldChanged(adapter.name, textAt(index))
            }

        }
    }
}
