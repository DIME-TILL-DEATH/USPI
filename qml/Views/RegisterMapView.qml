import QtQuick 2.12
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import Elements 1.0
import Fields 1.0
import Views 1.0

import "../CreateFunctions.js" as Scripts

Item {
    id: _root

    property alias fieldsView: _fieldsView
    property alias resultView: _text
    property alias registerMap: _registerMap

    Row{
        width: parent.width
        height: parent.height-_resultRectangle.height-padding-spacing

        padding: width / 200
        spacing: width / 200

        ListModel{
            id: _registerMap
        }

        ListView{
            id: _registerMapView

            width: parent.width * 0.15
            height: parent.height

            header:  Text {
                    id: _headerRegisterMapText
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: qsTr("Карта регистров:")
            }

            spacing: height/200

            model: _registerMap

            delegate: RegisterHeader{
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        _registerMapView.currentIndex = index
                        Scripts.createRegisterFields(_registerMapView.currentIndex, _root)
                    }
                }
            }
        }

        Column{
            id: _fieldsView

            property var registerAdapter

            width: parent.width * 0.6
            height: parent.height
            spacing: height/200
        }

        Rectangle{
            width: parent.width*0.1
            height: parent.height
            color: "transparent"
            border.width: 1
        }

        Column{
            width: parent.width * 0.15
            height: parent.height

            Text {
                id: _headerQueText
                anchors.horizontalCenter: parent.horizontalCenter

                text: qsTr("Порядок:")
            }

            ListView{
                width: parent.width
                height: parent.height-_headerQueText.height-_btnWrite.height
            }

            Button{
                id: _btnWrite
                width: parent.width*0.6

                anchors.horizontalCenter: parent.horizontalCenter

                text: "Записать"

                onPressed: {

                    var writeSequenceArray = []
                    writeSequenceArray.push(_registerMap.get(2).register)
                    writeSequenceArray.push(_registerMap.get(1).register)

                    Backend.registerWriteSequence = writeSequenceArray
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
