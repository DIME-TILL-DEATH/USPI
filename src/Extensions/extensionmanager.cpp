#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>

#include "controlpanelinterface.h"
#include "extensionmanager.h"

ExtensionManager::ExtensionManager(DUTDevice* controllingDevice, QObject *parent)
    :QObject{parent},
    m_controlledDevice{controllingDevice}
{

}

ExtensionManager::~ExtensionManager()
{

}

QStringList ExtensionManager::getPlugins(QString path)
{
    QStringList plugins;

    QStringList filter;
    filter  << "*.dll";
    QDir dir(path);

    qInfo() << "Searching plugins in: " << path;

    QFileInfoList list = dir.entryInfoList(filter);

    foreach(QFileInfo file, list)
    {
        plugins.append(file.filePath());
    }

    return plugins;
}

QStringList ExtensionManager::avaliablePlugInsNames()
{
    return m_pluginsList.keys();
}

void ExtensionManager::loadPlugins(std::vector<PluginInfo> pluginsList)
{
    m_loadedPlugInsInfo = pluginsList;
    foreach(PluginInfo pluginInfo, pluginsList)
    {
        loadPlugin(pluginInfo);
    }
}

std::vector<PluginInfo> ExtensionManager::loadedPlugInsInfo()
{
    for(auto info_it = m_loadedPlugInsInfo.begin(); info_it != m_loadedPlugInsInfo.end(); ++info_it)
    {
        (*info_it).setSettings(getPlugInSettings( (*info_it).name()));
    }

    return m_loadedPlugInsInfo;
}

void ExtensionManager::loadPlugin(PluginInfo pluginInfo)
{
    QString fileAddress = QFileInfo(pluginInfo.path()).absoluteFilePath();

    QPluginLoader* loader = new QPluginLoader(fileAddress, this);

    if(!loader->load())
    {
        qWarning() << "Не могу загрузить плагин: " << fileAddress << ", так как " << loader->errorString();
        return;
    }   

    ControlPanelInterface* plugin = qobject_cast<ControlPanelInterface*>(loader->instance());

    if(plugin)
    {
        connect(dynamic_cast<QObject*>(plugin), SIGNAL(setFieldValue(QString,QString,QVariant)),
                this, SLOT(setFieldValue(QString,QString,QVariant)));

        connect(dynamic_cast<QObject*>(plugin), SIGNAL(getFieldValue(QString,QString,QVariant&)),
                this, SLOT(getFieldValue(QString,QString,QVariant&)));

        connect(dynamic_cast<QObject*>(plugin), SIGNAL(writeRegisterSequence(QStringList)),
                this, SLOT(writeRegisterSequence(QStringList)));


        connect(this, SIGNAL(saveRequest(QString,QMap<QString,QString>&)),
                dynamic_cast<QObject*>(plugin), SLOT(saveRequest(QString,QMap<QString,QString>&)));

        connect(this, SIGNAL(loadRequest(QString, const QMap<QString,QString>&)),
                dynamic_cast<QObject*>(plugin), SLOT(loadRequest(QString, const QMap<QString,QString>&)));


        emit loadRequest(pluginInfo.name(), pluginInfo.settings());
    }
    else
    {
        qWarning() << "Could not cast: " << pluginInfo.name() << " : " << loader->fileName();
    }

    m_pluginsList.insert(pluginInfo.name(), loader);
}

void ExtensionManager::unloadPlugins()
{
    foreach(QPluginLoader* plugin, m_pluginsList)
    {
        plugin->unload();
        delete plugin;
    }
    m_pluginsList.clear();
}

void ExtensionManager::runPlugin(QString pluginName)
{
    if(!m_pluginsList.contains(pluginName))
    {
        qWarning() << "Plugin with name " << pluginName << " not found in list";
        return;
    }

    QPluginLoader* loader = m_pluginsList.value(pluginName);

    ControlPanelInterface* plugin = qobject_cast<ControlPanelInterface*>(loader->instance());

    if(plugin)
    {

        plugin->showPanel();
    }
    else
    {
        qWarning() << "Could not cast: " << pluginName << " : " << loader->fileName();
    }

    // в примере определяют простым перебором вариантов:
    //    if (plugin) {
    //             auto iBrush = qobject_cast<BrushInterface *>(plugin);
    //             if (iBrush)
    //                 addItems(pluginItem, "BrushInterface", iBrush->brushes());

    //             auto iShape = qobject_cast<ShapeInterface *>(plugin);
    //             if (iShape)
    //                 addItems(pluginItem, "ShapeInterface", iShape->shapes());

    //             auto iFilter = qobject_cast<FilterInterface *>(plugin);
    //             if (iFilter)
    //                 addItems(pluginItem, "FilterInterface", iFilter->filters());
    //         }
}

QMap<QString, QString> ExtensionManager::getPlugInSettings(const QString &plugInName)
{
    QMap<QString, QString> plugInSettings;

    emit saveRequest(plugInName, plugInSettings);

    return plugInSettings;
}

void ExtensionManager::setPlugInSettings(const QString &plugInName, const QMap<QString, QString> &plugInSettings)
{
    emit loadRequest(plugInName, plugInSettings);
}

void ExtensionManager::setFieldValue(QString registerName, QString fieldName, QVariant value)
{
    AbstractField* field = m_controlledDevice->findField(registerName, fieldName);

    if(field == nullptr) return;

    switch (field->type())
    {
        case AbstractField::FieldType::IntegerField:
        {
            IntegerField* intField = dynamic_cast<IntegerField*>(field);
            qulonglong setVal = value.toULongLong();

            if(intField) intField->setData(setVal);
            else qWarning() << "Не удается преобразовать тип поля";

            break;
        }

        case AbstractField::FieldType::BitField:
        {
            BitField* bitField = dynamic_cast<BitField*>(field);
            bool setVal = value.toBool();

            if(bitField) bitField->setBit(setVal);
            else qWarning() << "Не удается преобразовать тип поля";

            break;
        }

        case AbstractField::FieldType::VariantListField:
        {
            VariantListField* variantListField = dynamic_cast<VariantListField*>(field);
            QString setVal = value.toString();

            if(!variantListField->containsVariant(setVal)) qWarning() << "Ошибка установки VariantListField. Нет такого варианта в списке";

            if(variantListField) variantListField->setSelected(setVal);
            else qWarning() << "Не удается преобразовать тип поля";

            break;
        }

        default:
        {
            qWarning() << "ExtensionManager: ошибка записи в поле " << fieldName << " в регистре " << registerName;
            qWarning() << "Запись значений в данный тип поля не поддерживается";
        }
    }
}

void ExtensionManager::getFieldValue(QString registerName, QString fieldName, QVariant &value)
{
    AbstractField* field = m_controlledDevice->findField(registerName, fieldName);

    if(field == nullptr) return;

    switch (field->type())
    {
        case AbstractField::FieldType::IntegerField:
        {
            IntegerField* intField = dynamic_cast<IntegerField*>(field);

            if(intField) value = intField->data();
            else qWarning() << "Не удается преобразовать тип поля";

            break;
        }

        case AbstractField::FieldType::BitField:
        {
            BitField* bitField = dynamic_cast<BitField*>(field);

            if(bitField) value = bitField->getBit();
            else qWarning() << "Не удается преобразовать тип поля";

            break;
        }

        case AbstractField::FieldType::VariantListField:
        {
            VariantListField* variantListField = dynamic_cast<VariantListField*>(field);

            if(variantListField) value = variantListField->selected();
            else qWarning() << "Не удается преобразовать тип поля";

            break;
        }

        case AbstractField::FieldType::FixedField:
        {
            FixedField* fixedField = dynamic_cast<FixedField*>(field);

            if(fixedField) value = fixedField->data();
            else qWarning() << "Не удается преобразовать тип поля";

            break;
        }

        default:
        {
            qWarning() << "ExtensionManager: ошибка чтения поля " << fieldName << " в регистре " << registerName;
            qWarning() << "Чтение данного типа поля не поддерживается";
        }
    }
}

void ExtensionManager::writeRegisterSequence(QStringList registerNames)
{
    foreach(QString registerName, registerNames)
    {

    }
}
