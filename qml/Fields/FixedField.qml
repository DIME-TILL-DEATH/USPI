import QtQuick 2.15
import QtQuick.Controls 2.5

import Elements 1.0

Rectangle{
    id: _root

    property alias properties : _header

    width: parent.width
    height: parent.height/10
    border.width: 1
    radius: height/5

    FieldTip{
        tipText: properties.description
    }

    Row{
        width: parent.width
        height: parent.height

        spacing: width / 50

        FieldProperties{
            id: _header
        }

        ResultField{
            resultValue: properties.resultValue

            anchors.verticalCenter: parent.verticalCenter

            FontMetrics{
                id: _textMetrics
                font: font
            }
        }
    }
}
