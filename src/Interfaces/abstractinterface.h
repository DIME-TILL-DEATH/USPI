#ifndef ABSTRACTINTERFACE_H
#define ABSTRACTINTERFACE_H

#include <vector>
#include <memory>

#include <QByteArray>

#include "register.h"
#include "dutdevice.h"

#include "abstractcontroller.h"

class AbstractInterface : public QObject
{
public:
    AbstractInterface(QObject* parent=nullptr);
    virtual ~AbstractInterface() {};

    virtual bool writeRegister(Register* wrReg);
    virtual bool writeSequence(const std::vector<Register*> &wrSequence);

    virtual const QString &interfaceName() const;

    std::shared_ptr<AbstractController> selectedController();
    virtual std::vector<std::shared_ptr<AbstractController> >& connectedControllers();

    bool isAvaliable() const;

protected:
    std::vector<std::shared_ptr<AbstractController> > m_connectedControllers;
    bool m_isAvaliable{true};

    void reverseByte(char& byte);

private:
    QString m_interfaceName{"Abstract"};

};

#endif // ABSTRACTINTERFACE_H
