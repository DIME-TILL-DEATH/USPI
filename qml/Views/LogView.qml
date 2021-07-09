import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

Rectangle{
    id: _rootRectangle

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
                leftPadding: font.pixelSize
                anchors.verticalCenter: parent.verticalCenter
                text: "Лог событий:"
            }
        }

        delegate: Text{
            leftPadding: font.pixelSize

            text: model.display
        }
    }
}
