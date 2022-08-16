import QtQuick 2.15
import QtQml.Models 2.15

import StyleSettings 1.0

import "../CreateFunctions.js" as Scripts

MouseArea{
    id: _rootMA

    width: _registerMapView.width*0.9
    height: _registerMapView.height/10

    property bool dragEnabled: false
    property bool held: false

    drag.target: held ? contentRectangle : undefined
    drag.axis: Drag.YAxis

    pressAndHoldInterval: 200
    onPressAndHold:  held = (dragEnabled) ? true : false
    onReleased:
    {
        contentRectangle.Drag.drop()
        held = false
    }

//[root.components]
    DropArea {
        anchors { fill: parent;}

        onEntered: {
            var from = drag.source.DelegateModel.itemsIndex;
            var to = _rootMA.DelegateModel.itemsIndex;

            RegisterSequenceModel.moveItem(from, to)
        }

        onDropped: {

        }
    }

    Rectangle{
        id: contentRectangle

        width: _rootMA.width
        height: _rootMA.height
        opacity: _rootMA.held ? 0.8 : 1

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        border
        {
            width: held ? 3 : 1
            color: held ? "darkblue" : "black"
        }

        radius: width/10

        color: {
            if(register!==undefined){
                switch(register.registerType){
                   case "DUT": return parent.ListView.isCurrentItem ? Style.regHeaderActiveDUT : Style.regHeaderPassiveDUT
                   case "Controller": return parent.ListView.isCurrentItem ? Style.regHeaderActiveController : Style.regHeaderPassiveController
                   }
            }
            else
                return "white"
        }

        Drag.active: _rootMA.held
        Drag.source: _rootMA
        Drag.hotSpot.x: width / 2
        Drag.hotSpot.y: height / 2

        Text{
            text: register.name

            leftPadding: width/10
            rightPadding: width/10
            width: parent.width

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            elide: Text.ElideMiddle
            wrapMode: Text.WordWrap
            maximumLineCount: 2

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
        //[rect.states]
        states: State {
            when: _rootMA.held

            ParentChange { target: contentRectangle; parent: _root }
            AnchorChanges {
                target: contentRectangle
                anchors { horizontalCenter: undefined; verticalCenter: undefined }
            }
        }
    }
}
