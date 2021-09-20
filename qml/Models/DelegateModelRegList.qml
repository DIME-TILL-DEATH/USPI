import QtQuick 2.0
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Elements 1.0

import "../CreateFunctions.js" as Scripts

DelegateModel {
       id: registerListModel

       signal delegateClicked(registerAdapter : var)

       model: RegisterMapModel

       delegate: RegisterHeader{
               onClicked: {
                   // hard code!
                   _registerMapView.currentIndex = index
                   delegateClicked(register)
               }
       }
}
