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


    width:  (Backend === null) ? Screen.width*0.55
             : ((Backend.userSettings.windowWidth !==0) ? Backend.userSettings.windowWidth
                                                     : Screen.width*0.55)
    height: (Backend === null) ? Screen.height*0.65
            : ((Backend.userSettings.windowHeight !==0) ? Backend.userSettings.windowHeight
                                                     : Screen.height*0.65)

    minimumWidth: 640
    minimumHeight: 480

    visible: true
    title: qsTr("USPI")

    onClosing:
    {
        Backend.userSettings.windowWidth = width
        Backend.userSettings.windowHeight = height
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Файл")

            FileDialog {
                id: _sessionSaveDialog

                selectExisting: false

                title: "Выберите файл"
                nameFilters: [ "Файлы сессий (*.ses)", "All files (*)" ]

                onAccepted: {
                    var result = Backend.saveSession(fileUrl)
                }
                Component.onCompleted: visible = false
            }
            FileDialog {
                id: _sessionLoadDialog

                title: "Выберите файл"
                nameFilters: [ "Файлы сессий (*.ses)", "All files (*)" ]

                onAccepted: {
                    var result = Backend.loadSession(fileUrl)

                    Scripts.clearRegisterFields(_mainView)

                    _mainView.updateRegisterMap()
                    //!!!!!!!
                   // _mainView.registerView.registerView.currentIndex = -1
                   // _mainView.registerView.registerSequenceView.currentIndex = -1
                }
                Component.onCompleted: visible = false
            }

            Action {
                text: qsTr("&Сохранить проект")
                onTriggered:{
                    _sessionSaveDialog.visible = true
                }
            }
            Action {
                text: qsTr("&Открыть проект")
                onTriggered:{
                    _sessionLoadDialog.visible = true
                }
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

        mainView: _mainView
    }


    MainView{
        id: _mainView

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
        _mainView.resultView.text = "Результат: 0x" + _mainView.fieldsView.registerAdapter.value()

        if(_mainView.autoWrite)
        {
            Backend.writeSequence()
        }
    }
}
