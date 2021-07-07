import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import Fields 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("USPI")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&Open...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
    }

    Column{
        width: parent.width * 0.8
        height: parent.height

        spacing: height/200

        FixedField{

        }

        BitField{

        }

        IntegerField{

        }

        VariantListField{

        }
    }
}
