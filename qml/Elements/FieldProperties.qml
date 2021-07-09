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

    width: parent.width / 6
    height: parent.height

    border.width: 1
    radius: height/5

    Column{
        padding: _root.height/10

        anchors.verticalCenter: parent.verticalCenter

        Label{
            id: _fieldName

            text: name
        }

        Label{
            id: _bitsPostiton


            text: (size <= 1) ?  "Бит " + position + ":"
                                :"Биты " + (position+size) + "..." + position + ":"
        }
    }
}
