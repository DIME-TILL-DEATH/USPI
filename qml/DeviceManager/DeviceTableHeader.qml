import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import StyleSettings 1.0

Rectangle {
    id: _deviceTabHeader

    z:10

    color: Style.headerColor
    border.color: "black"
    border.width: 1

    signal sgBtnAddDeviceClicked()

    Button{
        id: _btnAddDevice

        x: parent.width*0.01

        anchors.verticalCenter: parent.verticalCenter

//        width: parent.width/10
        height: parent.height*0.8
        text: "Добавить устройство"

        background: Rectangle{
            border.width: 1
            border.color: Style.headerButtonBorderColor

            color: Style.headerButtonBackgroundColor
            opacity: _btnAddDevice.pressed ? 0.5 : 1
        }

        onClicked:{
            sgBtnAddDeviceClicked()
        }
    }
}
