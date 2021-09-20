import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import Elements 1.0

import "../CreateFunctions.js" as Scripts

Item{
    id: _root

    property alias delegate: _rootList.delegate
    property alias model: _rootList.model
    property alias currentIndex: _rootList.currentIndex
    property alias count: _rootList.count

    property string headerText : ""

//    signal delegateClicked(registerAdapter : var)

    width: parent.width
    height: parent.height

    Text {
        id: _headerRegisterMapText
        anchors.horizontalCenter: parent.horizontalCenter
        text: headerText
    }

    ListView{
        id: _rootList

        width: _root.width
        height: _root.height-_headerRegisterMapText.height

        anchors.top: _headerRegisterMapText.bottom

        model: _root.model

        boundsBehavior: Flickable.DragOverBounds
        clip: true

        ScrollBar.vertical: ScrollBar{}

        spacing: height/200

//        delegate: RegisterHeader{
//                onClicked: {
//                    _rootList.currentIndex = index
//                    delegateClicked(register)
//                }
//        }

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
