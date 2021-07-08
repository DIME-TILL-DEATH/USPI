import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

Rectangle{
    id: _rootRectangle

    anchors.bottom: parent.bottom
    width: parent.width
    height: parent.height *0.2

    border.width: 1


    ListView{
        id: _loggerWindow

        anchors.fill: parent

        model: Log

        header: Rectangle{
            width: _rootRectangle.width
            height: _text.font.pixelSize*2

            color: "azure"
            border.width: 1

            Text{
                id: _text
                leftPadding: parent.width/200
                anchors.verticalCenter: parent.verticalCenter
                text: "Лог событий:"
            }
        }

        delegate: Text{
            leftPadding: parent.width/200

            text: model.display
        }
    }
}
