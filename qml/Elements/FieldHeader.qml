import QtQuick 2.15
import QtQuick.Controls 2.12

Rectangle {
    id: _root

    property var adapter

    property int position : (adapter !== undefined) ? adapter.position : 0
    property int size : (adapter !== undefined) ? adapter.size : 0

    property string name: (adapter !== undefined) ? adapter.name : "Имя поля"
    property string description : (adapter !== undefined) ? adapter.description : "описание"
    property string comment: (adapter !== undefined) ? adapter.comment : "комментарий"

    width: parent.width / 4
    height: parent.height

    border.width: 1
    radius: height/5

    Column{
        padding: _root.height/10

        anchors.verticalCenter: parent.verticalCenter

        Text{
            id: _fieldName

            width: _root.width-parent.padding*2

            text: name
            elide: Text.ElideMiddle
        }

        Text{
            id: _bitsPostiton

            width: _root.width-parent.padding*2

            text: (size <= 1) ?  "Бит " + position + ":"
                                :"Биты " + (position+size-1) + "..." + position + ":"
        }
    }

    MouseArea{
        id: _mouseArea
        anchors.fill: parent
        onClicked: {

        }
    }
}
