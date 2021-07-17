import QtQuick 2.0

Item {
    property int baseMargins: parent.width/40
    anchors.fill: parent

    Text{
        text: qsTr("Виртуальный интерфейс.")

        anchors{
            top: parent.top
            left: parent.left
            margins: baseMargins
        }
    }
}
