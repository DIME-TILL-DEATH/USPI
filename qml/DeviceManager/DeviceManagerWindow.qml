import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import Elements 1.0
import StyleSettings 1.0

ApplicationWindow {
    id: _deviceManagerWindow

    visible: true
    flags: Qt.Dialog
    title: qsTr("Менеджер устройств")

    modality: Qt.WindowModal

    signal sgRefreshFields()

    header: DeviceTableHeader{
        id: _deviceTabHeader


        height: _deviceManagerWindow.height/8

        Connections{
            function onSgBtnAddDeviceClicked()
            {
                 _fileDeviceDialog.open();
            }
        }
    }

    ListView{
        id: _dutList

        width: parent.width*0.98
        height: parent.height*0.95

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: height/75

        boundsBehavior: Flickable.DragOverBounds
        clip: true

        model: DutListModel

//        headerPositioning: ListView.OverlayHeader

        delegate: DeviceTableDelegate{
            width: _dutList.width
            height: _dutList.height/6
        }

    }

    OpenFileDeviceDialog{
        id: _fileDeviceDialog

        onAccepted:{
            if(Backend.loadDevice(fileUrl))
            {
                sgRefreshFields();
            }
        }
    }
}

