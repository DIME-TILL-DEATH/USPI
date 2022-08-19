import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import QtQuick.Layouts 1.15

import StyleSettings 1.0
import Elements 1.0
import Fields 1.0
import Views 1.0
import Models 1.0

Rectangle{
    id: _root

    signal delegateClicked(index : int)

    color: "lightgray"

    ColumnLayout {
        anchors.fill: parent
        spacing: height/100

        Rectangle{
            color: "green"
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height*0.15
        }

        ListView{
            id: _dutList

            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: height/200

            boundsBehavior: Flickable.DragOverBounds
            clip: true

            model: DutListModel


            delegate: MouseArea{
                id: _rootMA

                width: _dutList.width//*0.9
                height: _dutList.height/5

                onClicked: {
                    _dutList.currentIndex = index
                    delegateClicked(index);
                }

                Rectangle{
                    anchors.fill: parent

                    color: _rootMA.ListView.isCurrentItem ? Style.regHeaderActiveDUT : Style.regHeaderPassiveDUT

                    radius: width/5
                    border.color: "black"
                    border.width: 1

                    Column{
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width
                        Text{
                            width: parent.width
                            elide: Text.ElideMiddle

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            text: deviceHeader.deviceName
                        }
                        Text{
                            width: parent.width
                            elide: Text.ElideMiddle

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            text: "Кан. №" + deviceHeader.channelNumber
                        }
                    }
                }
            }
        }

        Rectangle{
            color: "red"
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height*0.15
        }
    }
}

