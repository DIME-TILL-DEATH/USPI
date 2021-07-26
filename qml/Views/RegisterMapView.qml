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

    property bool autoWrite : _sequenceControlPanel.rbAuto.checked

    Row{
        id: _row

        width: parent.width
        height: parent.height-_resultRectangle.height-padding-spacing

        padding: width / 200
        spacing: width / 200

        function selectRegisterMap(registerAdapter)
        {
            Scripts.createRegisterFields(registerAdapter, _root)
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

        SequenceControlPanel{
            id: _sequenceControlPanel

            registerMapView: _registerMapView
            registerSequenceView: _registerSequenceView
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

                delegate: RegisterHeader{

                    color: isLocal ?
                               (ListView.isCurrentItem ? "green" : "grey") :
                               (ListView.isCurrentItem ? "skyblue" : "transparent")

                    MouseArea{
                        id: _ma
                        anchors.fill: parent
                        acceptedButtons: Qt.AllButtons
                        onClicked: {
                            if(mouse.button & Qt.RightButton)
                            {
                                _sequenceOptionsMenu.popup()
                            }
                            else
                            {
                                _registerSequenceView.currentIndex = index
                                if(isLocal)
                                {
                                    Scripts.createRegisterFields(register, _root)
                                }
                            }
                        }

                        Menu{
                            id: _sequenceOptionsMenu
                            MenuItem{
                                text: "индивидуально"
                                checkable: true
                                onTriggered: {
                                    Backend.changeWriteItemLocal(index)
                                }
                            }
            //                MenuSeparator{}
            //                MenuItem{
            //                    text: "пауза"
            //                }
                        }
                    }
                }
            }


            Button{
                id: _btnWrite

                width: parent.width*0.6
                anchors.horizontalCenter: parent.horizontalCenter

                enabled: _sequenceControlPanel.rbBtn.checked ? true : false

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
