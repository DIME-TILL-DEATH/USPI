#include "usbcontroller.h"

USBController::USBController(const QString &name, const QMap<QString, DevicePin> &devicePins)
    :AbstractController(name, devicePins)
{

}
