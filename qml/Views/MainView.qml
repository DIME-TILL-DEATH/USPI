import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15
import QtQuick.Layouts 1.15

import StyleSettings 1.0
import Elements 1.0
import Fields 1.0
import Views 1.0
import Models 1.0

import "../CreateFunctions.js" as Scripts

Item {
    id: _root

    property alias fieldsView: _fieldsView
    property alias resultView: _text
    property alias registerView: _regListView
    property alias registerSequenceView: _registerSequenceView

    property bool autoWrite : _sequenceControlPanel.rbAuto.checked

    function updateRegisterMap()
    {
        _sequenceControlPanel.regCount = _regListView.deviceRegisterListModel.count
        _sequenceControlPanel.currentRegList = _regListView.deviceRegisterListModel.model
        _sequenceControlPanel.currentIndex = -1
        _registerSequenceView.currentIndex = -1
    }

    Row{
        id: _row

        width: parent.width
        height: parent.height-_resultRectangle.height-padding-spacing

        padding: width / 200
        spacing: width / 200

        DutListView{
            id: _dutListView

            height: parent.height
            width: parent.width * 0.1

            Connections{
                function onDelegateClicked(index){
                    Backend.setCurrentRegisterMap(index)

                    _sequenceControlPanel.regCount = _regListView.registerView.count;
                    _sequenceControlPanel.currentIndex = _regListView.registerView.currentIndex;
                    _sequenceControlPanel.currentRegList = _regListView.registerView.listModel.model;
                }
            }
        }

        RegListView{
            id: _regListView

            height: parent.height
            width: parent.width * 0.1

            Connections{
                function onSelectRegister(registerAdapter, currentIndex)
                {
                    Scripts.createRegisterFields(registerAdapter, _root);
                    _sequenceControlPanel.currentIndex = currentIndex;
                    _flickable.contentY = 0;
                }
            }
        }

        Flickable{
            id: _flickable
            width: _row.width * 0.44
            height: _row.height

            contentHeight: (_fieldsView.children.length+1) * height/Style.fieldsOnScreen

            ScrollBar.vertical: ScrollBar{
                id: _flickScrollBar

                policy: (_flickable.contentHeight > _flickable.height) ?
                            ScrollBar.AlwaysOn : ScrollBar.AsNeeded
            }

            Column{
                id: _fieldsView

                property var registerAdapter

                width: _flickable.width - _flickScrollBar.width
                height: _flickable.height
                spacing: height/200
            }
        }

        SequenceControlPanel{
            id: _sequenceControlPanel

            registerSequenceView: _registerSequenceView
            currentRegList: CurrentRegMapModel
        }

        Column{
            width: parent.width * 0.14
            height: parent.height

            spacing: height/200
            padding: height/200

            RegisterList{
                id: _registerSequenceView

                headerText: qsTr("Порядок записи:")

                listModel: DelegateModelRegSequence{}

                height: parent.height-_btnWrite.height-parent.padding*4
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
