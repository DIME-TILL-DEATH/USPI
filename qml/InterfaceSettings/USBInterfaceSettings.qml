import QtQuick 2.0

Item {
    property int baseMargins: parent.width/40
    anchors.fill: parent

    Column{
        anchors{
            fill: parent
            margins: baseMargins
        }

        Text{
            text: qsTr("USB интерфейс.")
            height: font.pixelSize*2
        }

        Text{
            text: "Имя устройства: " + InterfaceSettings.usbInterfaceSettings.deviceName

            width: parent.width
            height: font.pixelSize*4

            horizontalAlignment: Text.AlignLeft

            maximumLineCount: 3
            elide: Text.ElideRight
            wrapMode: Text.Wrap
        }

        ListView{
            width: parent.width
            height: parent.height*0.5

            model: InterfaceSettings.usbInterfaceSettings.deviceInfo

            delegate: Text{
                text: modelData
            }
        }
    }
}
