import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import QtQuick.Dialogs 1.3

import Elements 1.0
import Fields 1.0
import Views 1.0

import "CreateFunctions.js" as Scripts

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("USPI")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Файл")
            Action {
                text: qsTr("&Открыть...")
                onTriggered: _fileDialog.open()
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Выход")
                onTriggered: Qt.quit()
            }
        }
    }

    FileDialog {
        id: _fileDialog

        title: "Выберите файл"
        nameFilters: [ "Файлы карты регистров (*.json)", "All files (*)" ]

        onAccepted: {
            var result = Backend.loadDevice(_fileDialog.fileUrl)

            if(result === "No error")
            {
                Log.message("Карта регистров успешно загружена")

                _registerMap.clear()
                for(var registerIndex=0; registerIndex < Backend.registerCount(); registerIndex++)
                {
                    _registerMap.append({"register" : Backend.getRegister(registerIndex)})
                }

                Scripts.createRegisterFields(0, _fieldsView)
            }
            else
            {
                Log.message(result)
            }
        }
        Component.onCompleted: visible = false
    }


    Row{
        anchors.fill: parent

        padding: width / 200
        spacing: width / 200

        ListModel{
            id: _registerMap
        }

        ListView{
            id: _registerMapView

            width: parent.width * 0.15
            height: parent.height

            spacing: height/200

            model: _registerMap

            delegate: RegisterHeader{
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        _registerMapView.currentIndex = index
                        Scripts.createRegisterFields(_registerMapView.currentIndex, _fieldsView)
                    }
                }
            }
        }

        Column{
            id: _fieldsView

            property var registerAdapter

            width: parent.width * 0.6
            height: parent.height
            spacing: height/200
        }

    }

    Rectangle{
        id: _resultRectangle

        anchors.bottom: _loggerWindow.top

        width: parent.width
        height: _text.font.pixelSize*2

        border.width: 1

        Text{
            id: _text
            leftPadding: font.pixelSize
            anchors.verticalCenter: parent.verticalCenter

            text: "Результат: 0x"
        }
    }

    LogView{
        id: _loggerWindow

        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height *0.2
    }

    function fieldChanged(fieldId, newValue){
//        console.log(fieldId, newValue)
        console.log(_fieldsView.registerAdapter.rawData())
    }
}
