import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import Elements 1.0
import Fields 1.0
import Views 1.0

import "../CreateFunctions.js" as Scripts

Rectangle{
    id: _root

    property int currentIndex : -1
    property int regCount : 0

    property var registerSequenceView
    property var currentRegList

    property int btnWidth : width*0.9
    property int btnHeight : height/10

    property alias rbAuto: _rbAuto
    property alias rbBtn: _rbBtn

    width: parent.width*0.18
    height: parent.height
    border.width: 1
    color: "transparent"

    Column{
        anchors.fill: parent
        spacing: parent.height/100
        padding: parent.height/50

        Button{
            width: btnWidth
            height: btnHeight
            anchors.horizontalCenter: parent.horizontalCenter

            text: ">"
            enabled: (currentIndex >= 0) ? true : false
            onPressed: {
                var index
                if(registerSequenceView.count>0) index = registerSequenceView.currentIndex+1
                else index = 0

                Backend.addRegisterToSequence(currentRegList.getItem(currentIndex), index);
                registerSequenceView.currentIndex += 1
            }
        }
        Button{
            width: btnWidth
            height: btnHeight
            anchors.horizontalCenter: parent.horizontalCenter

            text: ">>"
            //enabled: (registerMapView.count>0) ? true : false
            enabled: (regCount>0) ? true : false
            onPressed: {
                for(var i=regCount-1; i >= 0; i--)
                {
                    var index
                    if(registerSequenceView.count>0) index = registerSequenceView.currentIndex+1
                    else index = 0

                    Backend.addRegisterToSequence(currentRegList.getItem(i), index);
                }
            }
        }
        Button{
            width: btnWidth
            height: btnHeight
            anchors.horizontalCenter: parent.horizontalCenter

            text: "<"
            enabled: (registerSequenceView.currentIndex>-1) ? true : false
            onPressed: {
                //RegisterSequenceModel.removeItem(registerSequenceView.currentIndex)
                Backend.removeRegisterFromSequence(registerSequenceView.currentIndex);

                // топорное решение, придумать что-то получше
                // может быть сделать fieldsView моделью и передавать ему регистр
                // но главный вопрос, как универсальнее понять нужный индекс?
                if(registerSequenceView.currentIndex < registerSequenceView.count)
                {
                    Scripts.createRegisterFields(RegisterSequenceModel.getItem(registerSequenceView.currentIndex), _root.parent.parent)
                }
                else if(registerSequenceView.currentIndex === 0)
                {
                    Scripts.clearRegisterFields(_root.parent.parent)
                }
                else if(registerSequenceView.currentIndex === registerSequenceView.count && registerSequenceView.currentIndex !== 0)
                {
                    Scripts.createRegisterFields(RegisterSequenceModel.getItem(registerSequenceView.currentIndex-1), _root.parent.parent)
                }

            }
        }
        Button{
            width: btnWidth
            height: btnHeight
            anchors.horizontalCenter: parent.horizontalCenter

            enabled: (registerSequenceView.count>0) ? true : false
            text: "<<"
            onPressed: {
                dialog.open()
            }

            Dialog {
                id: dialog

                title: qsTr("Подтверждение удаления")
                standardButtons: Dialog.Ok | Dialog.Cancel
                modal: true

                Label {
                   text: "Вы действительно хотите удалить все регистры?"
                }

                onAccepted:
                {
                    for(var i=registerSequenceView.count-1; i >= 0; i--)
                    {
                         Backend.removeRegisterFromSequence(i);
                    }

                    if(registerSequenceView.currentIndex > -1)
                    {
                        Scripts.clearRegisterFields(_root.parent.parent)
                    }
                }

                //[transitions]
                enter: Transition {
                        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 250  }

                }
            }
        }

        Text{
            text: "Запись в устройство:"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        RadioButton{
            id: _rbBtn

            width: btnWidth
            height: btnHeight

            checked: true
            text: "По кнопке"
        }
        RadioButton{
            id: _rbAuto

            width: btnWidth
            height: btnHeight

            text: "По изменению"
        }
        CheckBox{
            id: _rbTrigger

            width: btnWidth
            height: btnHeight

            enabled: false
            text: "По триггеру"
        }
    }
}
