#include "fileinterface.h"

FileInterface::FileInterface()
{

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
        out << "Hexadecimal: 0x" << (*it)->rawData().toHex(m_hexSeparator.data()->toLatin1()) << "\n";

        out << "Binary: 0b";

        for(int i=0; i< (*it)->rawData().size() ; ++i)
        {
            char oneByte = (*it)->rawData().at(i);
            QString byteToBinary = QString("%1").arg(oneByte, 8, 2, QLatin1Char('0'));
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
}

const QString &FileInterface::binarySeparator() const
{
    return m_binarySeparator;
}

void FileInterface::setBinarySeparator(const QString &newBinarySeparator)
{
    m_binarySeparator = newBinarySeparator;
}

const QString &FileInterface::filePath() const
{
    return m_filePath;
}

void FileInterface::setFilePath(const QString &newFilePath)
{
    m_filePath = newFilePath;
}
