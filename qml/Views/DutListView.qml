import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import QtQuick.Layouts 1.15

import Globals 1.0
import StyleSettings 1.0
import Elements 1.0
import Fields 1.0
import Views 1.0
import Models 1.0

Rectangle{
    id: _root

    signal delegateClicked(index : int)

    border.width: 1

    ListView{
        id: _dutList

        width: parent.width
        height: parent.height

        topMargin: height/200
        bottomMargin: topMargin

        spacing: height/200

        boundsBehavior: Flickable.DragOverBounds
        clip: true

        model: DutListModel

        headerPositioning: ListView.OverlayHeader
        header: Rectangle{
            border.width: 1
            color: Style.labelColor

            width:_dutList.width
            height: _dutList.height/20
            z:10
            Text{
                text: qsTr("Устройства:")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        footerPositioning: ListView.OverlayHeader
        footer:     Button{
            id: _controllerBtn

            text: "Контроллер"

            width:_dutList.width
            height: _dutList.height/15
            z:10

            background: Rectangle{
                border.width: 1

                color: Style.headerButtonBackgroundColor
                opacity: _controllerBtn.pressed ? 0.5 : 1
            }

            onClicked: {
                _dutList.currentIndex = -1
                delegateClicked(-1)
            }
        }


        delegate: MouseArea{
            id: _rootMA

            width: _dutList.width
            height: _dutList.height/8

            onClicked: {
                _dutList.currentIndex = index
                delegateClicked(index);
            }

            Rectangle{
                width: parent.width*0.95
                height: parent.height

                anchors.horizontalCenter: parent.horizontalCenter

                color: _rootMA.ListView.isCurrentItem ? ((Status.selectedPanel === Status.Panels.RegisterMap) ? Style.regHeaderActiveDUT : Style.regMapPassive)
                                                      : Style.regHeaderPassiveDUT

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

//        //[transitions]
//        remove: Transition {
//                NumberAnimation { property: "opacity"; to: 0; duration: 500  }

//        }
//        removeDisplaced: Transition {
//                 NumberAnimation { properties: "y"; duration: 250 }
//        }
//        add: Transition {
//                 NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 500}
//        }
//        addDisplaced: Transition {
//                 NumberAnimation { properties: "y"; duration: 250 }
//                 NumberAnimation { properties: "opacity"; to: 1; duration: 500}
//        }

//        moveDisplaced: Transition {
//                 NumberAnimation { properties: "y"; duration: 150 }
//        }
    }
}

