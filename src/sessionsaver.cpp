#include "sessionsaver.h"

SessionSaver::SessionSaver(DUTDevice *device, RegisterListModel *registerMapModel, RegisterListModel *registerWriteSequenceModel)
    :m_device{device},
     m_registerMapModel{registerMapModel},
     m_registerWriteSequenceModel{registerWriteSequenceModel}
{

}

bool SessionSaver::saveSession(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QDataStream outFile(&file);

        outFile << m_sessionSaverVersion;
        outFile << *m_device;

        size_t writeSequenceSize = m_registerWriteSequenceModel->registerAdaptersList().size();
        outFile << writeSequenceSize;

        for(auto it = m_registerWriteSequenceModel->registerAdaptersList().begin();
                it != m_registerWriteSequenceModel->registerAdaptersList().end();
                ++it)
        {
            outFile << (*it).getRegister()->uniqueId();
        }

        qDebug() << "Сессия" << filePath << "сохранена";
        file.close();
        return true;
    }
    else
    {
        // или всё-таки Logger вручную? Привести к единному варианту
        qInfo() << "Невозможно открыть файл " << filePath;
        return false;
    }
}

bool SessionSaver::loadSession(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QDataStream inFile(&file);

        inFile >> m_sessionSaverVersion;
        inFile >> *m_device;

        m_registerMapModel->resetModel(m_device->deviceRegisterMap());

        size_t writeSequenceSize;
        inFile >> writeSequenceSize;

        m_registerWriteSequenceModel->resetModel();

        for(quint16 i=0; i<writeSequenceSize; ++i)
        {
            quint16 registerUniqueId;
            inFile >> registerUniqueId;

            Register* reg_ptr = m_device->registerByUniqueId(registerUniqueId);

            if(reg_ptr != nullptr) m_registerWriteSequenceModel->addItem(RegisterAdapter(reg_ptr), i);
        }
        file.close();
        qInfo() << "Сессия" << filePath << "загружена";
        return true;
    }
    else
    {
        // или всё-таки Logger вручную? Привести к единному варианту
        qWarning() << "Невозможно открыть файл " << filePath;
        return false;
    }
}

