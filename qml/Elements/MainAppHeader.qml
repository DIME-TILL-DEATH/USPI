import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import QtQuick.Dialogs 1.3

import StyleSettings 1.0

import Elements 1.0
import Fields 1.0
import Views 1.0
import InterfaceSettings 1.0

Rectangle{
        id: _headerRect

        property var openFileDeviceDialog

        border.width: 1
        color: Style.headerColor

        Row{
            anchors.fill: parent

            padding: width / 100
            spacing: width / 100

            Text{
                text: "Устройство:"
                anchors.verticalCenter: parent.verticalCenter
            }

            Button{
                id: _btnChooseDevice

                height: _headerRect.height*0.8
                anchors.verticalCenter: parent.verticalCenter

                text: (Backend !== null) ? Backend.dutDeviceName : ""

                background: Rectangle{
                    border.width: 1
                    border.color: Style.headerButtonBorderColor

                    color: Style.headerButtonBackgroundColor
                    opacity: _btnChooseDevice.pressed ? 0.5 : 1
                }
                onPressed: {
                    openFileDeviceDialog.open()
                }
            }

            Text{
                // заменить символьную стрелку на красивую картинку)
                text: "  ->    Интерфейс:"
                anchors.verticalCenter: parent.verticalCenter
            }

            Button{
                id: _btnChooseInterface

                width: parent.width/5
                height: _headerRect.height*0.8
                anchors.verticalCenter: parent.verticalCenter

                text: if(Backend != null) Backend.currentInterface

                background: Rectangle{
                    border.width: 1
                    border.color: Style.headerButtonBorderColor

                    color: Style.headerButtonBackgroundColor
                    opacity: _btnChooseInterface.pressed ? 0.5 : 1
                }

                onPressed: {
                    _interfaceSettingsWindowLoader.active = true
                }
            }

            Loader{
                id: _interfaceSettingsWindowLoader
                active: false


                sourceComponent: InterfaceSettingsWindow{

                    width: _headerRect.parent.width/2
                    height: _headerRect.parent.height/2

                    onClosing: _interfaceSettingsWindowLoader.active = false
                }
            }
        }
    }
