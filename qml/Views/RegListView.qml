import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import StyleSettings 1.0
import Elements 1.0
import Fields 1.0
import Views 1.0
import Models 1.0

import "../CreateFunctions.js" as Scripts

Row {
    property alias registerView: _registerMapView
    property alias deviceRegisterListModel: _deviceRegisterListModel

    padding: width / 50
    spacing: width / 25

    TabBar{
        id: _tabSelectMap

        height: parent.height
        width: parent.width * 0.15

        TabButton{
            id:  control
            text: qsTr("Устройство")

        }
        TabButton{
            text: qsTr("Контроллер")
        }

        contentItem: ListView {
                id: _listView

                model: ListModel{
                    ListElement{ tabText: "Устройство"}
                    ListElement{ tabText: "Контроллер"}
                }

                currentIndex: _tabSelectMap.currentIndex


                spacing: height/200
                orientation: ListView.Vertical   // <<-- VERTICAL

                delegate:
                Button{
                    id: _delegateButton
                    width: _listView.width
                    height: _listView.height / 6

                    Text {
                        id: _btnText

                         text: tabText


                        transform: Rotation {
                            origin.x: t_metrics.tightBoundingRect.width/2
                            origin.y: t_metrics.tightBoundingRect.height/2
                            angle: -90}

                        anchors.horizontalCenter:  _delegateButton.horizontalCenter
                        anchors.verticalCenter:  _delegateButton.verticalCenter

                        // opacity: enabled ? 1.0 : 0.3
                     }

                    TextMetrics {
                            id:     t_metrics
                            font:   _btnText.font
                            text:   _btnText.text
                        }

                     background: Rectangle {
                       //  opacity: enabled ? 1 : 0.3
                         color: (_tabSelectMap.currentIndex === index) ? "lightgreen" : "lightgray"
                         border.color: control.down ? "#17a81a" : "#21be2b"
                         border.width: 1
                         radius: 4
                     }
                     onClicked: {_tabSelectMap.currentIndex = index}
                }
        }
    }


    StackLayout{
        width: parent.width * 0.8
        height: parent.height
        currentIndex: _tabSelectMap.currentIndex
        Item{
            RegisterList{
                id: _registerMapView

                headerText: qsTr("Регистры устройства:")
                model: DelegateModelRegList{
                    id: _deviceRegisterListModel
                }
            }
        }
    }
}
