import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.15

import Elements 1.0
import StyleSettings 1.0

Item{
    id: _rootRect

    required property int index
    required property var deviceHeader

    Rectangle{
        id: _backgroundRect

        width: parent.width*0.98
        height: parent.height

        anchors.horizontalCenter: parent.horizontalCenter

        radius: width/200
        border.color: "black"
        border.width: 1
    }

    DropShadow{
        anchors.fill: _backgroundRect
        horizontalOffset: 3
        verticalOffset: 2
        radius: 6.0
        samples: 17
        color: "#80000000"
        source: _backgroundRect
        cached: true
    }

    Row{
        anchors.centerIn: parent.Center

        width:parent.width*0.97
        height:parent.height

        spacing: width/50

        Label{
            width: parent.width*0.15
            anchors.verticalCenter: parent.verticalCenter

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: deviceHeader.deviceName
        }
        Label{
            width: parent.width*0.52
            elide: Text.ElideMiddle
            anchors.verticalCenter: parent.verticalCenter

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: deviceHeader.description
        }

        ComboBox{
            id: _channelSelector
            width: parent.width*0.2
            anchors.verticalCenter: parent.verticalCenter

            currentIndex: _rootRect.deviceHeader.channelNumber

            background: Rectangle {
                border.color: _channelSelector.down ? Style.borderColorActive : Style.borderColorPassive
                border.width: _channelSelector.visualFocus ? 2 : 1
                radius: 2
            }

            model: Backend.avaliableChannels

            onActivated: {
                Backend.setChannelForDevice(_rootRect.index, index)
            }
        }
        Button{
            width: parent.width*0.05
            anchors.verticalCenter: parent.verticalCenter
            text: "X"
        }
    }
}

