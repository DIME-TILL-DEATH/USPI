#include "usbcontroller.h"

USBController::USBController(const QString &name, const QMap<QString, DevicePin> &devicePins)
    :AbstractController(name, devicePins)
{

}

QStringList USBController::avaliableChannels()
{
    return QStringList{{"Кан.№0 - SPI"}, {"Кан.№1 - SPI"}, {"Кан.№2 - SPI"}, {"Кан.№3 - SPI"}};
}
