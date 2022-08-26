import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import QtQuick.Dialogs 1.3

import StyleSettings 1.0

import DeviceManager 1.0
import Elements 1.0
import Fields 1.0
import Views 1.0
import InterfaceSettings 1.0

Rectangle{
        id: _headerRect

        property var openFileDeviceDialog

        signal openDeviceManager()

        border.width: 1
        color: Style.headerColor

        Row{
            anchors.fill: parent

            padding: width / 100
            spacing: width / 100

//            Text{
//                text: "Устройство:"
//                anchors.verticalCenter: parent.verticalCenter
//            }

            Button{
                id: _btnChooseDevice

                height: _headerRect.height*0.8
                anchors.verticalCenter: parent.verticalCenter

                text: "Менеджер устройств"//(Backend !== null) ? Backend.dutDeviceName : ""

                background: Rectangle{
                    border.width: 1
                    border.color: Style.headerButtonBorderColor

                    color: Style.headerButtonBackgroundColor
                    opacity: _btnChooseDevice.pressed ? 0.5 : 1
                }
                onPressed: {
                    openDeviceManager()
//                    openFileDeviceDialog.open()
//                    _deviceManagerWindow.active = true
                }
            }

            Text{
                // заменить символьную стрелку на красивую картинку)
                text: "  ->    Интерфейс:"
                anchors.verticalCenter: parent.verticalCenter
            }

            Button{
                id: _btnChooseInterface

                width: parent.width/10
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

            ComboBox{
                id: _pluginSelector

                width: parent.width/3.8
                height: _headerRect.height*0.8

                anchors.verticalCenter: parent.verticalCenter
                background: Rectangle {
                         border.color: _pluginSelector.down ? Style.borderColorActive : Style.borderColorPassive
                         border.width: _pluginSelector.visualFocus ? 2 : 1
                         radius: 2
                 }

                model: Backend.avaliablePlugins
            }

            Button{
                id: _btnRunPlugin

                width: parent.width/10
                height: _headerRect.height*0.8
                anchors.verticalCenter: parent.verticalCenter

                text: "Запуск"

                background: Rectangle{
                    border.width: 1
                    border.color: Style.headerButtonBorderColor

                    color: Style.headerButtonBackgroundColor
                    opacity: _btnRunPlugin.pressed ? 0.5 : 1
                }

                onPressed: {
//                    Backend.runPlugin(_pluginSelector.currentText)
                    Backend.runPlugin(_pluginSelector.currentIndex)
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
