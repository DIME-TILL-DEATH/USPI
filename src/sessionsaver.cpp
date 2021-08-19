#include "sessionsaver.h"

SessionSaver::SessionSaver(DUTDevice *device, std::vector<std::shared_ptr<Register> >* localRegisterMap, RegisterListModel *registerMapModel, RegisterListModel *registerWriteSequenceModel)
    :m_device{device},
      m_localRegisterMap{localRegisterMap},
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

            bool isLocalRegister = (*it).isLocal();
            outFile << isLocalRegister;

            if(isLocalRegister)
            {
                outFile << *((*it).getRegister());
            }
        }

        qInfo() << "Сессия" << filePath << "сохранена";
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

            std::shared_ptr<Register> reg_ptr;


            bool isLocal;
            inFile >> isLocal;

            if(isLocal)
            {
                Register* localRegister_ptr = new Register;
                inFile >> *localRegister_ptr;
                m_localRegisterMap->push_back(std::shared_ptr<Register>(localRegister_ptr));
                reg_ptr = m_localRegisterMap->back();
            }
            else
            {
                reg_ptr = m_device->registerByUniqueId(registerUniqueId);
            }
            RegisterAdapter regAdapter(reg_ptr);

            regAdapter.setIsLocal(isLocal);

            if(reg_ptr != nullptr) m_registerWriteSequenceModel->addItem(regAdapter, i);
        }
        file.close();
        qInfo() << "Сессия" << filePath << "загружена";
        return true;
    }
    else
    {
        qWarning() << "Невозможно открыть файл " << filePath;
        return false;
    }
}

