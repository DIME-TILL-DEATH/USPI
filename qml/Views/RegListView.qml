import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import QtQuick.Layouts 1.15

import DUTDevice 1.0

import StyleSettings 1.0
import Elements 1.0
import Fields 1.0
import Views 1.0
import Models 1.0

import "../CreateFunctions.js" as Scripts

RegisterList{
    id: _registerMapView

    property alias registerView: _registerMapView
    property alias deviceRegisterListModel: _deviceRegisterListModel

    signal selectRegister(registerAdapter : var, index : int)

    width: parent.width
    height: parent.height

    headerText: qsTr("Регистры:")

    listModel: DelegateModelRegList{
        id: _deviceRegisterListModel

        model: CurrentRegMapModel
        modelView: _registerMapView

    }

    Connections{
        target: _deviceRegisterListModel
        function onDelegateClicked(registerAdapter, currentIndex)
        {
            selectRegister(registerAdapter, currentIndex);
        }
    }
}
