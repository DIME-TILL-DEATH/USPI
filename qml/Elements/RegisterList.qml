import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0

import "../CreateFunctions.js" as Scripts

Rectangle{
    id: _root

    property alias delegate: _rootList.delegate
    property alias listModel: _rootList.model
    property alias currentIndex: _rootList.currentIndex
    property alias count: _rootList.count

    property string headerText : ""

    width: parent.width
    height: parent.height

    border.width: 1

    ListView{
        id: _rootList

        anchors.fill: parent

        topMargin: height/200
        bottomMargin: topMargin

        model: _root.model

        boundsBehavior: Flickable.DragOverBounds
        clip: true

        ScrollBar.vertical: ScrollBar{}

        spacing: height/200

        headerPositioning: ListView.OverlayHeader
        header: Rectangle{
            z:10

            border.width: 1
            color: Style.labelColor
            width:_rootList.width
            height: _rootList.height/20
            Text{
                text: headerText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        //[transitions]
        remove: Transition {
                NumberAnimation { property: "opacity"; to: 0; duration: 500  }

        }
        removeDisplaced: Transition {
                 NumberAnimation { properties: "y"; duration: 250 }
        }
        add: Transition {
                 NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 500}
        }
        addDisplaced: Transition {
                 NumberAnimation { properties: "y"; duration: 250 }
                 NumberAnimation { properties: "opacity"; to: 1; duration: 500}
        }

        moveDisplaced: Transition {
                 NumberAnimation { properties: "y"; duration: 150 }
        }
    }
}
