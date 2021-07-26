import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Dialogs 1.3

import Elements 1.0
import Fields 1.0
import Views 1.0

import "../CreateFunctions.js" as Scripts

FileDialog {
    id: _fileDialog

    property var registerMapView

    title: "Выберите файл"
    nameFilters: [ "Файлы карты регистров (*.json)", "All files (*)" ]

    onAccepted: {
        if(Backend.loadDevice(_fileDialog.fileUrl))
        {
            Scripts.createRegisterFields(RegisterMapModel.getItem(0), registerMapView)
        }
    }
    Component.onCompleted: visible = false
}
