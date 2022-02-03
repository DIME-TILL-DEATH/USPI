import QtQuick 2.12
import QtQuick.Controls 2.15

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
        _sequenceControlPanel.regCount = _regListView.regMaps[_regListView.currentMapIndex].count
        _sequenceControlPanel.currentRegList = _regListView.regMaps[_regListView.currentMapIndex].listModel.model
        _sequenceControlPanel.currentIndex = -1

        _registerSequenceView.currentIndex = -1
    }

    Row{
        id: _row

        width: parent.width
        height: parent.height-_resultRectangle.height-padding-spacing

        padding: width / 200
        spacing: width / 200



        function selectRegister(registerAdapter, currentIndex)
        {
            Scripts.createRegisterFields(registerAdapter, _root)
            _sequenceControlPanel.currentIndex = currentIndex
            _flickable.contentY = 0
        }

        function selectMap(selectedRegisterList)
        {
            _sequenceControlPanel.regCount = selectedRegisterList.count
            _sequenceControlPanel.currentIndex = selectedRegisterList.currentIndex
            _sequenceControlPanel.currentRegList = selectedRegisterList.listModel.model
        }

        Component.onCompleted: {
            _regListView.deviceRegisterListModel.delegateClicked.connect(selectRegister)
            _regListView.controllerRegisterListModel.delegateClicked.connect(selectRegister)

            _regListView.selectRegMap.connect(selectMap)
        }

        RegListView{
            id: _regListView

            height: parent.height
            width: parent.width * 0.15
        }


        Flickable{
            id: _flickable
            width: _row.width * 0.47
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
            currentRegList: RegisterMapModel
        }

        Column{
            width: parent.width * 0.15
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
