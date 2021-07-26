import QtQuick 2.12

import Elements 1.0

import "../CreateFunctions.js" as Scripts

ListView{
    id: _rootList

    property string headerText : ""

    signal delegateClicked(registerAdapter : var)

    width: parent.width
    height: parent.height

    headerPositioning: ListView.OverlayHeader
    header:  Text {
            id: _headerRegisterMapText
            anchors.horizontalCenter: parent.horizontalCenter

            text: headerText
    }

    boundsBehavior: Flickable.DragOverBounds
    clip: true

    spacing: height/200

    delegate: RegisterHeader{
        MouseArea{
            anchors.fill: parent
            onClicked: {
                _rootList.currentIndex = index
                delegateClicked(register)
            }
        }
    }

    Component.onCompleted: {
        currentIndex = 0
    }

    //==========================================================
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
}
