import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import StyleSettings 1.0

Rectangle{
    id: _rootRectangle

    border.width: 1

    Rectangle{
        id: _header

        width: _rootRectangle.width
        height: _text.font.pixelSize*2

        color: Style.logHeader
        border.width: 1

        Text{
            id: _text
            leftPadding: font.pixelSize
            anchors.verticalCenter: parent.verticalCenter
            text: "Лог событий:"
        }
    }


    ListView{
        id: _loggerWindow

        clip: true

        width: parent.width
        height: parent.height - _header.height

        anchors.top: _header.bottom

        model: Log

        ScrollBar.vertical: ScrollBar{
            policy: ScrollBar.AlwaysOn
        }

        delegate: Text{

            width: _loggerWindow.width

            leftPadding: font.pixelSize

            text: msgText
            color: msgColor

            elide: Text.ElideRight
            wrapMode: Text.WordWrap

            Component.onCompleted: {
                _loggerWindow.positionViewAtEnd()
            }
        }

    }
}
