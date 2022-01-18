#include "usbcontroller.h"

USBController::USBController(AbstractInterface *interface, const QString &name, const QMap<QString, DevicePin> &devicePins)
    :AbstractController(interface, name, devicePins)
{

}
