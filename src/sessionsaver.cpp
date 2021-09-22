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
    if(file.open(QIODevice::WriteOnly))
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

        // added to v.0.2 Saving field adapters. Added view options
        for(auto reg_it = m_registerMapModel->registerAdaptersList().begin();
                 reg_it != m_registerMapModel->registerAdaptersList().end();
                 ++reg_it)
        {
            for(auto field_it = (*reg_it).m_bufferedFieldAdapters.begin();
                     field_it != (*reg_it).m_bufferedFieldAdapters.end();
                     ++field_it)
            {
                // view mode of field (bin, dec, hex)
                outFile << field_it.key();
                outFile << (*field_it).viewOptions();
            }
        }

        for(auto reg_it = m_registerWriteSequenceModel->registerAdaptersList().begin();
                 reg_it != m_registerWriteSequenceModel->registerAdaptersList().end();
                 ++reg_it)
        {
            for(auto field_it = (*reg_it).m_bufferedFieldAdapters.begin();
                     field_it != (*reg_it).m_bufferedFieldAdapters.end();
                     ++field_it)
            {
                // view mode of field (bin, dec, hex)
                outFile << field_it.key();
                outFile << (*field_it).viewOptions();
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
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream inFile(&file);

        QString saverVersion;
        inFile >> saverVersion;

        saverVersion.remove(0, 2);
        saverVersion = saverVersion.left(3);
        m_compareVersion = saverVersion.toDouble();


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

        // view options
        if(m_compareVersion > 0.1)
        {
            for(auto reg_it = m_registerMapModel->registerAdaptersList().begin();
                     reg_it != m_registerMapModel->registerAdaptersList().end();
                     ++reg_it)
            {
                for(auto field_it = (*reg_it).m_bufferedFieldAdapters.begin();
                         field_it != (*reg_it).m_bufferedFieldAdapters.end();
                         ++field_it)
                {
                    // view mode of field (bin, dec, hex)
                    quint16 index;
                    quint16 viewBase;

                    inFile >> index;
                    inFile >> viewBase;

                    (*reg_it).m_bufferedFieldAdapters[index].setViewOptions(viewBase);
                }
            }

            for(auto reg_it = m_registerWriteSequenceModel->registerAdaptersList().begin();
                     reg_it != m_registerWriteSequenceModel->registerAdaptersList().end();
                     ++reg_it)
            {
                for(auto field_it = (*reg_it).m_bufferedFieldAdapters.begin();
                         field_it != (*reg_it).m_bufferedFieldAdapters.end();
                         ++field_it)
                {
                    // view mode of field (bin, dec, hex)
                    quint16 index;
                    quint16 viewBase;

                    inFile >> index;
                    inFile >> viewBase;

                    (*reg_it).m_bufferedFieldAdapters[index].setViewOptions(viewBase);
                }
            }
        }

        file.close();
        qInfo() << "Сессия" << filePath << "загружена. Версия формата: " << m_compareVersion;
        return true;
    }
    else
    {
        qWarning() << "Невозможно открыть файл " << filePath;
        return false;
    }
}

