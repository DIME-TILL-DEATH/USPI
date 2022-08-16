import QtQuick 2.0
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Elements 1.0

import "../CreateFunctions.js" as Scripts

DelegateModel {
       id: registerSequenceModel

       model: RegisterSequenceModel

       delegate: RegisterHeader{
           id: _ma

           width: _registerSequenceView.width*0.9
           height: _registerSequenceView.height/10

           dragEnabled: true

           onClicked: {
                   // hard code!
                   _registerSequenceView.currentIndex = DelegateModel.itemsIndex
                   Scripts.createRegisterFields(register, _root)
           }

       }
}

