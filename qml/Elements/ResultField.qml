import QtQuick 2.0
import QtQuick.Controls 2.0

Label{
    property int resultValue

    // сделать по правой кнопке мыши выбор отображения между двоичным, шестнадцетеричным и десятичным доменом
    text: "0x" + resultValue.toString(16)


}
