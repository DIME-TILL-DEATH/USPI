import QtQuick 2.15
import QtQuick.Controls 2.5

import Elements 1.0

Rectangle{
    id: _root

    property var adapter

    property int value: (adapter !== undefined) ? adapter.value : 0

    signal fieldChanged(fieldId : string, newValue : int)

    width: parent.width
    height: parent.height/10
    border.width: 1
    radius: height/5

    FieldTip{
        tipText: (adapter !== undefined) ? adapter.description : "описание"
    }

    Row{
        width: parent.width
        height: parent.height

        spacing: width / 50

        FieldProperties{
            adapter: _root.adapter
        }

        Label{
            text: "0x" + _root.value.toString(16)

            anchors.verticalCenter: parent.verticalCenter

            FontMetrics{
                id: _textMetrics
                font: font
            }
        }
    }
}
