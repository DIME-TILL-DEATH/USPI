import QtQuick 2.12
import QtQuick.Controls 2.15

Item {
    property int baseMargins: parent.width/40
    anchors.fill: parent

    Column{
        anchors{
            fill: parent
            margins: baseMargins
        }

        Text{
            id: _text1

            text: qsTr("USB интерфейс.")
            height: font.pixelSize*2
        }

        Text{
            id: _text2

            text: "Имя устройства: " + InterfaceSettings.usbInterfaceSettings.deviceName

//            font.family: "Arial"

            width: parent.width
            height: font.pixelSize*4

            horizontalAlignment: Text.AlignLeft

            maximumLineCount: 3
            elide: Text.ElideRight
            wrapMode: Text.Wrap
        }

        ListView{
            width: parent.width
            height: parent.height - _text1.height - _text2.height - 10

            model: InterfaceSettings.usbInterfaceSettings.deviceInfo

            ScrollBar.vertical: ScrollBar{
                policy: ScrollBar.AlwaysOn
            }

            delegate: Text{
                text: modelData
            }
        }
    }
}
