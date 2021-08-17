import QtQuick 2.15

import StyleSettings 1.0

import "../CreateFunctions.js" as Scripts

Rectangle{
    width: _registerMapView.width*0.9
    height: _registerMapView.height/10

    border.width: 1
    radius: width/10

    color: ListView.isCurrentItem ? Style.regHeaderActiveGlobal : Style.regHeaderPassiveGlobal

    Text{
        text: register.name
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
