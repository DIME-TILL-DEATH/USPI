import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import InterfaceSettings 1.0

ApplicationWindow {

    visible: true
    flags: Qt.Dialog
    title: qsTr("Interface settings")

    property int baseMargins: width/50

    function showSettings(index)
    {
        switch(_comboInterfaces.textAt(index))
        {
            case "File": _settingsPageLoader.source = "FileInterfaceSettings.qml"; break;
            case "USB": _settingsPageLoader.source = "USBInterfaceSettings.qml"; break;
            case "Abstract": _settingsPageLoader.source = "AbstractInterfaceSettings.qml"; break;
            default: _settingsPageLoader.sourceComponent = undefined
        }

    }

    Component.onCompleted: {
        _comboInterfaces.currentIndex = _comboInterfaces.find(Backend.currentInterface)
        showSettings(_comboInterfaces.currentIndex)
    }

    ComboBox{
       id: _comboInterfaces

      anchors{
          top: parent.top
          left: parent.left
          margins: baseMargins
      }

       model: Backend.avaliableInterfaces

       onActivated: {
           Backend.currentInterface = textAt(index)
           showSettings(index)
       }
    }

    Button{

       anchors{
           horizontalCenter: _comboInterfaces.horizontalCenter
           top: _comboInterfaces.bottom
           margins: baseMargins
       }

       text: qsTr("Обновить")

       onPressed: {
          Backend.updateAvaliableInterfaces()
       }
    }

    Rectangle{
        id: _rectSettings

        border.width: 1

        width: parent.width-baseMargins*3-_comboInterfaces.width
        height: parent.height-baseMargins*2
        anchors{
            top: parent.top
            left: _comboInterfaces.right
            margins: baseMargins
        }

        Loader{
            id: _settingsPageLoader

            anchors.fill: parent
            active: true
        }
    }
}
