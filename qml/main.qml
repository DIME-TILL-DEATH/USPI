import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import QtQuick.Dialogs 1.3

import Fields 1.0
import Views 1.0

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
                Log.message("Карта регистров успешно загружена")
            else
                Log.message(result)

            var reg = Backend.getRegister()
            for(var fieldIndex=0; fieldIndex<reg.fieldsCount; fieldIndex++)
            {
                var fieldAdapter = reg.field(fieldIndex)
                createField(fieldAdapter)
            }

        }
        Component.onCompleted: visible = false
    }

    Column{
        id: _columnFields

        width: parent.width * 0.8
        height: parent.height

        spacing: height/200
    }

    LogView{}

    function createField(fieldAdapter) {
        var fieldTypeName
        switch(fieldAdapter.type){
            case "fixed" :
                fieldTypeName = "Fields/FixedField.qml";
                break;
            case "bit" :
                fieldTypeName = "Fields/BitField.qml";
                break;
            case "integer" :
                fieldTypeName = "Fields/IntegerField.qml";
                break;
            case "variant_list":
                fieldTypeName = "Fields/VariantListField.qml";
                break;
            default:
                Log.message("Неизвестный тип поля")
                break;
        }
        var component = Qt.createComponent(fieldTypeName);
        var field = component.createObject(_columnFields);

        if (field === null) {
            Log.message("Ошибка создания поля");
        }
        else
        {
            field.fieldChanged.connect(fieldChanged)

            field.properties.name = fieldAdapter.name
            field.properties.description = fieldAdapter.description
            field.properties.comment = fieldAdapter.comment

            field.properties.position = fieldAdapter.position
            field.properties.size = fieldAdapter.size

            field.fieldIndex = fieldAdapter.index
        }
    }

    function fieldChanged(fieldId, newValue){
        console.log(fieldId, newValue)
    }
}
