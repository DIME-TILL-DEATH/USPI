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
    id: _appWindow

    width: 800
    height: 600
    visible: true
    title: qsTr("USPI")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Файл")
            Action {
                text: qsTr("&Открыть проект")
                onTriggered:{}
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Выход")
                onTriggered: Qt.quit()
            }
        }
    }

    header: MainAppHeader{
        width: _appWindow.width
        height: _appWindow.height/15

        openFileDeviceDialog: _fileDeviceDialog
    }

    OpenFileDeviceDialog{
        id: _fileDeviceDialog

        registerMapView: _registerMapView
    }


    RegisterMapView{
        id: _registerMapView

        width: parent.width
        height: parent.height *0.8
    }

    LogView{
        id: _loggerWindow

        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height *0.2
    }

    function fieldChanged(fieldId, newValue){
        _registerMapView.resultView.text = "Результат: 0x" + _registerMapView.fieldsView.registerAdapter.value()
    }
}
