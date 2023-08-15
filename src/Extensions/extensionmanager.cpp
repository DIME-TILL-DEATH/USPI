#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>

#include "controlpanelinterface.h"
#include "extensionmanager.h"

ExtensionManager::ExtensionManager(QObject *parent)
    :QObject{parent}
{

}

ExtensionManager::~ExtensionManager()
{

}

QStringList ExtensionManager::getPlugins(const QString &path)
{
    QStringList plugins;

    QStringList filter;
    filter  << "*.dll";
    QDir dir(path);

    qInfo() << QObject::tr("Searching plugins in: ") << path;

    QFileInfoList list = dir.entryInfoList(filter);

    foreach(QFileInfo file, list)
    {
        plugins.append(file.filePath());
    }

    return plugins;
}

QStringList ExtensionManager::avaliablePlugIns()
{
    QStringList result;
    foreach(PluginInfo pluginInfo, m_loadedPlugInsInfo)
    {
        result.append(pluginInfo.name() + tr(" - кан.№") + QString("%1").arg(pluginInfo.targetDevice()->channelNumber()));
    }
    return result;
}

void ExtensionManager::loadPlugins(const std::vector<PluginInfo> &pluginsList)
{
   // m_loadedPlugInsInfo = pluginsList;

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

void ExtensionManager::loadPlugin(PluginInfo &pluginInfo)
{
    QString fileAddress = QFileInfo(pluginInfo.path()).absoluteFilePath();

    QPluginLoader* loader = new QPluginLoader(fileAddress, this);

    if(!loader->load())
    {
        qWarning() << tr("Не могу загрузить плагин: ") << fileAddress << tr(", так как ") << loader->errorString();
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

        connect(dynamic_cast<QObject*>(plugin), SIGNAL(writeRegisterSequence()),
                this, SLOT(writeRegisterSequence()));


        connect(this, SIGNAL(saveRequest(QString,QMap<QString,QString>&)),
                dynamic_cast<QObject*>(plugin), SLOT(saveRequest(QString,QMap<QString,QString>&)));

        connect(this, SIGNAL(loadRequest(QString, const QMap<QString,QString>&)),
                dynamic_cast<QObject*>(plugin), SLOT(loadRequest(QString, const QMap<QString,QString>&)));


        emit loadRequest(pluginInfo.name(), pluginInfo.settings());
    }
    else
    {
        qWarning() << "Could not cast: " << pluginInfo.name() << " : " << loader->fileName();
        return;
    }

    plugin->setTargetDeviceNumber(m_loadedPlugInsInfo.size());
    pluginInfo.setLoader(loader);

    // Для того, чтобы для одного и того же dll создавались разные копии плагина для устройства
    // необходимо чтобы САМ плагин был фабрикой по созданию копий
    // те, возможно, в интерфейсе стоит заменить showPanel на Create
    m_loadedPlugInsInfo.push_back(pluginInfo);
}

void ExtensionManager::unloadPlugin(PluginInfo &pluginInfo)
{
    pluginInfo.loader()->unload();

    DUTDevice* targetDevice = pluginInfo.targetDevice();
    auto plugIt = std::find_if(m_loadedPlugInsInfo.begin(), m_loadedPlugInsInfo.end(), [targetDevice](PluginInfo seekPluginInfo)
                                                                                         {return seekPluginInfo.targetDevice() == targetDevice;});
    m_loadedPlugInsInfo.erase(plugIt);
}

void ExtensionManager::unloadAllPlugins()
{
    foreach(auto plugin, m_loadedPlugInsInfo)
    {
        plugin.loader()->unload();
    }
    m_loadedPlugInsInfo.clear();
}


void ExtensionManager::runPlugin(quint16 number)
{
    if(number > m_loadedPlugInsInfo.size())
    {
        qWarning() << "Номер расширения больше чем число доступных расширений";
        return;
    }

    QPluginLoader* loader= m_loadedPlugInsInfo.at(number).loader();
    ControlPanelInterface* plugin= qobject_cast<ControlPanelInterface*>(loader->instance());
    // Костыль для случая множества копий одного и того же плагина
    // для разных плагинов не нужен
    plugin->setTargetDeviceNumber(number);

    if(plugin)
    {
        plugin->showPanel();
    }
    else
    {
        qWarning() << tr("Could not cast: ") << m_loadedPlugInsInfo.at(number).name() << " : " << loader->fileName();
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
    ControlPanelInterface* plugin = qobject_cast<ControlPanelInterface*>(sender());

    if(plugin)
    {
        DUTDevice* targetDevice = m_loadedPlugInsInfo.at(plugin->targetDeviceNumber()).targetDevice();
        AbstractField* field = targetDevice->findField(registerName, fieldName);

        if(field == nullptr) return;

        switch (field->type())
        {
            case AbstractField::FieldType::IntegerField:
            {
                IntegerField* intField = dynamic_cast<IntegerField*>(field);
                qulonglong setVal = value.toULongLong();

                if(intField) intField->setData(setVal);
                else qWarning() << tr("Не удается преобразовать тип поля");

                break;
            }

            case AbstractField::FieldType::BitField:
            {
                BitField* bitField = dynamic_cast<BitField*>(field);
                bool setVal = value.toBool();

                if(bitField) bitField->setBit(setVal);
                else qWarning() << tr("Не удается преобразовать тип поля");

                break;
            }

            case AbstractField::FieldType::VariantListField:
            {
                VariantListField* variantListField = dynamic_cast<VariantListField*>(field);
                QString setVal = value.toString();

                if(!variantListField->containsVariant(setVal)) qWarning() << tr("Ошибка установки VariantListField. Нет такого варианта в списке");

                if(variantListField) variantListField->setSelected(setVal);
                else qWarning() << tr("Не удается преобразовать тип поля");

                break;
            }

            default:
            {
                qWarning() << tr("ExtensionManager: ошибка записи в поле ") << fieldName << tr(" в регистре ") << registerName;
                qWarning() << tr("Запись значений в данный тип поля не поддерживается");
            }
        }
    }
    else
    {
        qWarning() << tr("ExtensionManager: не удаётся преобразовать QObject в ControlPanelInterface");
    }
}

void ExtensionManager::getFieldValue(QString registerName, QString fieldName, QVariant &value)
{
    ControlPanelInterface* plugin = qobject_cast<ControlPanelInterface*>(sender());

    if(plugin)
    {
        DUTDevice* targetDevice = m_loadedPlugInsInfo.at(plugin->targetDeviceNumber()).targetDevice();

        AbstractField* field = targetDevice->findField(registerName, fieldName);

        if(field == nullptr) return;

        switch (field->type())
        {
            case AbstractField::FieldType::IntegerField:
            {
                IntegerField* intField = dynamic_cast<IntegerField*>(field);

                if(intField) value = intField->data();
                else qWarning() << tr("Не удается преобразовать тип поля");

                break;
            }

            case AbstractField::FieldType::BitField:
            {
                BitField* bitField = dynamic_cast<BitField*>(field);

                if(bitField) value = bitField->getBit();
                else qWarning() << tr("Не удается преобразовать тип поля");

                break;
            }

            case AbstractField::FieldType::VariantListField:
            {
                VariantListField* variantListField = dynamic_cast<VariantListField*>(field);

                if(variantListField) value = variantListField->selected();
                else qWarning() << tr("Не удается преобразовать тип поля");

                break;
            }

            case AbstractField::FieldType::FixedField:
            {
                FixedField* fixedField = dynamic_cast<FixedField*>(field);

                if(fixedField) value = fixedField->data();
                else qWarning() << tr("Не удается преобразовать тип поля");

                break;
            }

            default:
            {
                qWarning() << tr("ExtensionManager: ошибка чтения поля ") << fieldName << tr(" в регистре ") << registerName;
                qWarning() << tr("Чтение данного типа поля не поддерживается");
            }
        }
    }
    else
    {
        qWarning() << tr("ExtensionManager: не удаётся преобразовать QObject в ControlPanelInterface");
    }
}

void ExtensionManager::writeRegisterSequence(QStringList registerNames)
{
    //qInfo() << "Call writeRegisterSequence, object: " << sender()->objectName();

    ControlPanelInterface* plugin = qobject_cast<ControlPanelInterface*>(sender());

    if(plugin)
    {
        //qInfo() << "qobject_cast success";
        DUTDevice* targetDevice = m_loadedPlugInsInfo.at(plugin->targetDeviceNumber()).targetDevice();
        emit writeCustomSequenceRequest(registerNames, targetDevice);
    }
}

void ExtensionManager::writeRegisterSequence()
{
    emit writeSequenceRequest();
}
