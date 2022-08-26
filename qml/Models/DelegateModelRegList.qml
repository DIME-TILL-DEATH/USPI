import QtQuick 2.0
import QtQml.Models 2.15
import QtQuick.Controls 2.15

import Elements 1.0
import Globals 1.0

DelegateModel {
       id: registerListModel

       property var modelView

       signal delegateClicked(registerAdapter : var, index : int)

       delegate: RegisterHeader{
           panelType: Status.Panels.RegisterMap
               onClicked: {
                   modelView.currentIndex = index
                   delegateClicked(register, index)
                   Status.selectedPanel = Status.Panels.RegisterMap
               }
       }
}
