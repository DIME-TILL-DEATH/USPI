import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import Elements 1.0
import Fields 1.0
import Views 1.0

import "../CreateFunctions.js" as Scripts

Item {
    id: _root

    property alias fieldsView: _fieldsView
    property alias resultView: _text
    property alias registerView: _registerMapView

    property bool autoWrite : _rbAuto.checked

    Row{
        id: _row

        width: parent.width
        height: parent.height-_resultRectangle.height-padding-spacing

        padding: width / 200
        spacing: width / 200

        function selectRegisterMap(index)
        {
            Scripts.createRegisterFields(index, _root)
        }
        Component.onCompleted: {
            _registerMapView.delegateClicked.connect(selectRegisterMap)
        }


        RegisterList{
            id: _registerMapView

            headerText: qsTr("Карта регистров:")

            width: parent.width * 0.15
            model: RegisterMapModel
        }

        Column{
            id: _fieldsView

            property var registerAdapter

            width: parent.width * 0.47
            height: parent.height
            spacing: height/200
        }

        Rectangle{
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
                    enabled: (_registerMapView.count>0) ? true : false
                    onPressed: {
                        RegisterSequenceModel.addItem(RegisterMapModel.getItem(_registerMapView.currentIndex), _registerSequenceView.currentIndex)
                    }
                }
                Button{
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: ">>"
                    enabled: (_registerMapView.count>0) ? true : false
                    onPressed: {
                        for(var i=_registerMapView.count-1; i >= 0; i--)
                        {
                            RegisterSequenceModel.addItem(RegisterMapModel.getItem(i), _registerSequenceView.currentIndex)
                        }
                    }
                }
                Button{
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "<"
                    enabled: (_registerSequenceView.count>0) ? true : false
                    onPressed: {
                        RegisterSequenceModel.removeItem(_registerSequenceView.currentIndex)
                    }
                }
                Button{
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: (_registerSequenceView.count>0) ? true : false
                    text: "<<"
                    onPressed: {
                        for(var i=_registerSequenceView.count-1; i >= 0; i--)
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

        Column{
            width: parent.width * 0.15
            height: parent.height

            spacing: height/200
            padding: height/200

            RegisterList{
                id: _registerSequenceView

                headerText: qsTr("Порядок записи:")

                height: parent.height-_btnWrite.height-parent.padding*4
                model: RegisterSequenceModel
            }

            Button{
                id: _btnWrite

                width: parent.width*0.6
                anchors.horizontalCenter: parent.horizontalCenter

                enabled: _rbBtn.checked ? true : false

                text: "Записать"

                onPressed: {
                    Backend.writeSequence()
                }
            }
        }

    }

    Rectangle{
        id: _resultRectangle

        anchors.bottom: parent.bottom

        width: parent.width
        height: _text.font.pixelSize*2

        border.width: 1

        Text{
            id: _text
            leftPadding: font.pixelSize
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
