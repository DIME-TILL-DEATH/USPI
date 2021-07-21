import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: _root

    property int baseMargins: parent.width/40
    anchors.fill: parent

    Text {
        id: _header

        anchors{
            top: parent.top
            left: parent.left
            margins: baseMargins
        }

        text: qsTr("Запсись в файл:")
    }

    Grid{
        anchors{
            top: _header.bottom
            left: parent.left
            margins: baseMargins
        }

        rows: 3
        columns: 2

        rowSpacing: baseMargins
        columnSpacing: baseMargins

        Text{
            id: _headerFilePath
            text: "Путь к файлу"


        }
        TextField{
            id: _fieldFilePath

            height: _headerFilePath.height*2
            width: _root.width * 0.4

            text: InterfaceSettings.fileInterfaceSettings.filePath

            onTextChanged: {
                InterfaceSettings.fileInterfaceSettings.filePath = text
            }

            onAccepted: {
                focus = false
            }
        }

        Text{
            text: "Hex разделитель"
        }
        TextField{
            text: InterfaceSettings.fileInterfaceSettings.hexSeparator

            height: _headerFilePath.height*2
            width: _root.width * 0.4

            maximumLength: 1

            onTextChanged: {
                InterfaceSettings.fileInterfaceSettings.hexSeparator = text
            }
            onAccepted: {
                focus = false
            }
        }

        Text{
            text: "Binary разделитель"
        }
        TextField{
            text: InterfaceSettings.fileInterfaceSettings.binSeparator

            height: _headerFilePath.height*2
            width: _root.width * 0.4

            maximumLength: 1

            onTextChanged: {
                InterfaceSettings.fileInterfaceSettings.binSeparator = text
            }
            onAccepted: {
                focus = false
            }
        }
    }
}
