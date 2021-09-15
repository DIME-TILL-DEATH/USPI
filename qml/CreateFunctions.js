function createField(fieldAdapter, container)
{
    var fieldTypeName
    switch(fieldAdapter.type){
        case "fixed" :
            fieldTypeName = "Fields/FixedField.qml";
            break;
        case "bit" :
            fieldTypeName = "Fields/BitField.qml";
            break;
        case "integer" :
            fieldTypeName = "Fields/IntegerField.qml";
            break;
        case "variant_list":
            fieldTypeName = "Fields/VariantListField.qml";
            break;
        default:
            Log.message("Неизвестный тип поля")
            break;
    }
    var component = Qt.createComponent(fieldTypeName);
    var field = component.createObject(container);

    if (field === null) {
        Log.message("Ошибка создания поля");
    }
    else
    {
        field.fieldChanged.connect(fieldChanged)
        field.adapter = fieldAdapter
    }
}

function clearRegisterFields(registerMapView)
{
    for(var childIndex=0; childIndex < registerMapView.fieldsView.children.length; childIndex++)
    {
        registerMapView.fieldsView.children[childIndex].destroy()
    }
}

function createRegisterFields(registerAdapter, registerMapView)
{
    clearRegisterFields(registerMapView);

    registerMapView.fieldsView.registerAdapter = registerAdapter

    for(var fieldIndex = registerAdapter.fieldsCount-1; fieldIndex >= 0; fieldIndex--)
    {
        var fieldAdapter = registerAdapter.field(fieldIndex)
        Scripts.createField(fieldAdapter, registerMapView.fieldsView)
    }
    registerMapView.resultView.text = "Результат: 0x" + registerAdapter.value()
}
