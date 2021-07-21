#include "fileinterface.h"

FileInterface::FileInterface()
{
    m_settings.beginGroup("FileInterface");
    m_filePath = m_settings.value("filePath", "result.txt").toString();
    m_hexSeparator = m_settings.value("hexSeparator", "").toString();
    m_binarySeparator = m_settings.value("binarySeparator", "").toString();
    m_settings.endGroup();
}

bool FileInterface::writeSequence(const std::vector<Register *> &wrSequence)
{
    QFile file(m_filePath);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);

    QTextCodec *codec = QTextCodec::codecForName("UTF-16");
    QTextCodec::setCodecForLocale(codec);
    out.setCodec(codec);

    out << QString("Целевое устройство: ") << m_deviceHeader.deviceName << "\n";
    out << QString("Размер регистров: ") << m_deviceHeader.registerSize << QString(" бит\n");
    out << QString("Порядок следования: ") << QString(m_deviceHeader.isMSB ? "Старшим битов вперёд(MSB)" : "Младшим битов вперёд LSB") << "\n\n";

    for(auto it = wrSequence.begin(); it != wrSequence.end(); ++it)
    {
        out << (*it)->name() << ":\n";

        QByteArray rawData = (*it)->rawData();

        if(m_deviceHeader.isMSB) std::reverse(rawData.begin(), rawData.end());

        out << "Hexadecimal: 0x" << rawData.toHex(m_hexSeparator.data()->toLatin1()) << "\n";

        out << "Binary: 0b";

        for(int i=0; i< rawData.size() ; ++i)
        {
            uchar oneByte = rawData.at(i);
            QString byteToBinary = QString("%1").arg(oneByte, 8, 2, QLatin1Char('0'));

             if(!m_deviceHeader.isMSB) std::reverse(byteToBinary.begin(), byteToBinary.end());

            out << byteToBinary << m_binarySeparator;
        }
        out << "\n";
    }

    file.close();
    return true;
}

const QString &FileInterface::interfaceName() const
{
    return m_interfaceName;
}

const QString &FileInterface::hexSeparator() const
{
    return m_hexSeparator;
}

void FileInterface::setHexSeparator(const QString &newHexSeparator)
{
    m_hexSeparator = newHexSeparator;
    m_settings.setValue("FileInterface/hexSeparator", m_hexSeparator);
}

const QString &FileInterface::binarySeparator() const
{
    return m_binarySeparator;
}

void FileInterface::setBinarySeparator(const QString &newBinarySeparator)
{
    m_binarySeparator = newBinarySeparator;
    m_settings.setValue("FileInterface/binarySeparator", m_binarySeparator);
}

const QString &FileInterface::filePath() const
{
    return m_filePath;
}

void FileInterface::setFilePath(const QString &newFilePath)
{
    m_filePath = newFilePath;
    m_settings.setValue("FileInterface/filePath", m_filePath);
}
