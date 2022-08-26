pragma Singleton
import QtQuick 2.0

QtObject {
    enum Panels{
        None,
        RegisterMap,
        SequenceMap
    }

    property var selectedPanel: Status.Panels.None
}
