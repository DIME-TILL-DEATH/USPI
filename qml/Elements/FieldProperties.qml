import QtQuick 2.15
import QtQuick.Controls 2.12

Rectangle {
    id: _root

    property int position : 0
    property int size : 0

    property int resultValue: 0

    property string name: "Имя поля"
    property string description : "описание"
    property string comment: "комментарий"

    width: parent.width / 8
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


            text: (size <= 1) ?  "Бит " + position
                                :"Биты " + (position+size) + "..." + position + ":"
        }
    }
}
