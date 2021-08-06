#include "interfacessettingsadapter.h"

using namespace USB;

InterfacesSettingsAdapter::InterfacesSettingsAdapter( QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent)
    : QObject(parent),
      m_avaliableInterfaces{avaliableInterfaces}
{

}

void InterfacesSettingsAdapter::registerTypes()
{
    qRegisterMetaType<FileInterfaceSettings>("FileInterfaceSettings");
    qRegisterMetaType<USBInterfaceSettings>("USBInterfaceSettings");
}

FileInterfaceSettings InterfacesSettingsAdapter::fileInterfaceSettings()
{
    FileInterfaceSettings answer;

    answer.m_filePath = "result.txt";
    answer.m_binSeparator = "";
    answer.m_hexSeparator = "";

    QString interfaceName = FileInterface().interfaceName();
    AbstractInterface* abstract_ptr = m_avaliableInterfaces->value(interfaceName);

    FileInterface* fileInterface_ptr = dynamic_cast<FileInterface*>(abstract_ptr);

    if(fileInterface_ptr != nullptr)
    {
        answer.m_filePath = fileInterface_ptr->filePath();
        answer.m_binSeparator = fileInterface_ptr->binarySeparator();
        answer.m_hexSeparator = fileInterface_ptr->hexSeparator();
    }
    return answer;
}

void InterfacesSettingsAdapter::setFileInterfaceSettings(const FileInterfaceSettings &newSettings)
{
    FileInterface* fileInterface_ptr = dynamic_cast<FileInterface*>(m_avaliableInterfaces->value(FileInterface().interfaceName()));

    if(fileInterface_ptr != nullptr)
    {
        fileInterface_ptr->setFilePath(newSettings.m_filePath);
        fileInterface_ptr->setBinarySeparator(newSettings.m_binSeparator);
        fileInterface_ptr->setHexSeparator(newSettings.m_hexSeparator);
    }
}

USBInterfaceSettings InterfacesSettingsAdapter::usbInterfaceSettings()
{
    USBInterfaceSettings answer;

    AbstractInterface* abstract_ptr = m_avaliableInterfaces->value(USB::interfaceNameString);

    USBInterface* usbInterface_ptr = dynamic_cast<USBInterface*>(abstract_ptr);

    if(usbInterface_ptr != nullptr)
    {
        answer.deviceName = usbInterface_ptr->activeDevice().deviceName;
        answer.deviceInfo = usbInterface_ptr->activeDevice().deviceInfo;
    }
    return answer;
}

void InterfacesSettingsAdapter::setUSBInterfaceSettings(const USBInterfaceSettings &newSettings)
{

}
