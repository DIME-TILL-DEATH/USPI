pragma Singleton
import QtQuick 2.0

QtObject {

    readonly property color borderColorActive:    "blue"
    readonly property color borderColorPassive:   "gray"

    readonly property color regHeaderActiveGlobal  : "skyblue"
    readonly property color regHeaderPassiveGlobal : "white"
    readonly property color regHeaderActiveLocal : "lightgreen"
    readonly property color regHeaderPassiveLocal : "lightgray"

    readonly property color separationField  : "azure"

    readonly property color logHeader  : "azure"

    readonly property color headerButtonBorderColor: "blue"
    readonly property color headerButtonBackgroundColor: "azure"
    readonly property color headerColor: "palegreen"

    readonly property int fieldsOnScreen: 10
}
