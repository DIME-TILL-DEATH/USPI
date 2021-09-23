#ifndef FILEINTERFACE_H
#define FILEINTERFACE_H

#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>

#include <QSettings>

#include "abstractinterface.h"

class FileInterface : public AbstractInterface
{
public:
    FileInterface();

    bool writeSequence(const std::vector<Register*> &wrSequence) override;

    const QString &interfaceName() const override;

    const QString &hexSeparator() const;
    void setHexSeparator(const QString& newHexSeparator);
    const QString &binarySeparator() const;
    void setBinarySeparator(const QString &newBinarySeparator);
    const QString &filePath() const;
    void setFilePath(const QString &newFilePath);

private:
    QString m_interfaceName{"File"};

    QString m_hexSeparator;
    QString m_binarySeparator;
    QString m_filePath;

    QSettings m_settings;
};

#endif // FILEINTERFACE_H
