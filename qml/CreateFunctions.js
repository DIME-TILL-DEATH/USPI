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

function createRegisterFields(registerIndex, container)
{
    for(var childIndex=0; childIndex < container.children.length; childIndex++)
    {
        container.children[childIndex].destroy()
    }

    var reg = Backend.getRegister(registerIndex)
    container.registerAdapter = reg

    for(var fieldIndex = reg.fieldsCount-1; fieldIndex >= 0; fieldIndex--)
    {
        var fieldAdapter = reg.field(fieldIndex)
        Scripts.createField(fieldAdapter, container)
    }
}
