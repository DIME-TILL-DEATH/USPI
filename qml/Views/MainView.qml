import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import StyleSettings 1.0
import Elements 1.0
import Fields 1.0
import Views 1.0
import Models 1.0

import "../CreateFunctions.js" as Scripts

Item {
    id: _root

    property alias fieldsView: _fieldsView
    property alias resultView: _text
    property alias registerView: _regListView
    property alias registerSequenceView: _registerSequenceView

    property bool autoWrite : _sequenceControlPanel.rbAuto.checked

    Row{
        id: _row

        width: parent.width
        height: parent.height-_resultRectangle.height-padding-spacing

        padding: width / 200
        spacing: width / 200

        function selectRegisterMap(registerAdapter)
        {
            Scripts.createRegisterFields(registerAdapter, _root)
            _flickable.contentY = 0
           // _registerSequenceView.currentIndex = -1
        }

        Component.onCompleted: {
            _regListView.deviceRegisterListModel.delegateClicked.connect(selectRegisterMap)
        }

        RegListView{
            id: _regListView

            height: parent.height
            width: parent.width * 0.15
        }

//        TabBar{
//            id: _tabSelectMap

//            height: parent.height
//            width: parent.width * 0.02

//            TabButton{
//                id:  control
//                text: qsTr("Устройство")

//            }
//            TabButton{
//                text: qsTr("Контроллер")
//            }

//            contentItem: ListView {
//                    id: _listView

//                    model: ListModel{
//                        ListElement{ tabText: "Устройство"}
//                        ListElement{ tabText: "Контроллер"}
//                    }

//                    currentIndex: _tabSelectMap.currentIndex


//                    spacing: height/200
//                    orientation: ListView.Vertical   // <<-- VERTICAL

//                    delegate:
//                    Button{
//                        id: _delegateButton
//                        width: _listView.width
//                        height: _listView.height / 6

//                        Text {
//                            id: _btnText

//                             text: tabText


//                            transform: Rotation {
//                                origin.x: t_metrics.tightBoundingRect.width/2
//                                origin.y: t_metrics.tightBoundingRect.height/2
//                                angle: -90}

//                            anchors.horizontalCenter:  _delegateButton.horizontalCenter
//                            anchors.verticalCenter:  _delegateButton.verticalCenter

//                            // opacity: enabled ? 1.0 : 0.3
//                         }

//                        TextMetrics {
//                                id:     t_metrics
//                                font:   _btnText.font
//                                text:   _btnText.text
//                            }

//                         background: Rectangle {
//                           //  opacity: enabled ? 1 : 0.3
//                             color: (_tabSelectMap.currentIndex === index) ? "lightgreen" : "lightgray"
//                             border.color: control.down ? "#17a81a" : "#21be2b"
//                             border.width: 1
//                             radius: 4
//                         }
//                         onClicked: {_tabSelectMap.currentIndex = index}
//                    }
//            }
//        }


//        StackLayout{
//            width: parent.width * 0.15
//            height: parent.height
//            currentIndex: _tabSelectMap.currentIndex
//            Item{
//                RegisterList{
//                    id: _registerMapView

//                    headerText: qsTr("Регистры устройства:")
//                    model: DelegateModelRegList{
//                        id: _registerListModel
//                    }
//                }
//            }
//        }

        Flickable{
            id: _flickable
            width: _row.width * 0.47
            height: _row.height

            contentHeight: (_fieldsView.children.length+1) * height/Style.fieldsOnScreen

            ScrollBar.vertical: ScrollBar{
                id: _flickScrollBar

                policy: (_flickable.contentHeight > _flickable.height) ?
                            ScrollBar.AlwaysOn : ScrollBar.AsNeeded
            }

            Column{
                id: _fieldsView

                property var registerAdapter

                width: _flickable.width - _flickScrollBar.width
                height: _flickable.height
                spacing: height/200
            }
        }

        SequenceControlPanel{
            id: _sequenceControlPanel

            registerMapView: _regListView.registerView
            registerSequenceView: _registerSequenceView
        }

        Column{
            width: parent.width * 0.15
            height: parent.height

            spacing: height/200
            padding: height/200

            RegisterList{
                id: _registerSequenceView

                headerText: qsTr("Порядок записи:")

                model: DelegateModelRegSequence{}

                height: parent.height-_btnWrite.height-parent.padding*4
            }


            Button{
                id: _btnWrite

                width: parent.width*0.6
                anchors.horizontalCenter: parent.horizontalCenter

                enabled: _sequenceControlPanel.rbBtn.checked ? true : false

                text: "Записать"

                onPressed: {
                    Backend.writeSequence()
                }
            }
        }

    }

    Rectangle{
        id: _resultRectangle

        anchors.bottom: parent.bottom

        width: parent.width
        height: _text.font.pixelSize*2

        border.width: 1

        Text{
            id: _text
            leftPadding: font.pixelSize
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
