import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import Elements 1.0
import Fields 1.0
import Views 1.0

Rectangle{
    property var registerMapView
    property var registerSequenceView

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
            anchors.horizontalCenter: parent.horizontalCenter
            text: ">"
            enabled: (registerMapView.count>0) ? true : false
            onPressed: {
                RegisterSequenceModel.addItem(RegisterMapModel.getItem(registerMapView.currentIndex), registerSequenceView.currentIndex)
            }
        }
        Button{
            anchors.horizontalCenter: parent.horizontalCenter
            text: ">>"
            enabled: (registerMapView.count>0) ? true : false
            onPressed: {
                for(var i=_registerMapView.count-1; i >= 0; i--)
                {
                    RegisterSequenceModel.addItem(RegisterMapModel.getItem(i), registerSequenceView.currentIndex)
                }
            }
        }
        Button{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "<"
            enabled: (registerSequenceView.count>0) ? true : false
            onPressed: {
                RegisterSequenceModel.removeItem(registerSequenceView.currentIndex)
            }
        }
        Button{
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: (registerSequenceView.count>0) ? true : false
            text: "<<"
            onPressed: {
                for(var i=registerSequenceView.count-1; i >= 0; i--)
                {
                    RegisterSequenceModel.removeItem(i)
                }
            }
        }

        Text{
            text: "Запись в устройство:"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        RadioButton{
            id: _rbBtn
            checked: true
            text: "По кнопке"
        }
        RadioButton{
            id: _rbAuto
            text: "По изменению"
        }
        CheckBox{
            id: _rbTrigger
            enabled: false
            text: "По триггеру"
        }
    }
}