#include <algorithm>

#include "devicepin.h"

DevicePin::DevicePin(const QString &name,
                     const std::vector<Direction> &avaliableDirections,
                     const std::vector<Function> &avaliableFunctions)
    : m_name{name},
      m_avaliableDirections{avaliableDirections},
      m_avaliableFunctions{avaliableFunctions}
{

}

QMap<DevicePin::Function, DevicePin::Direction> DevicePin::pinDirForFunction()
{
    QMap<DevicePin::Function, DevicePin::Direction> directions;

    directions[Function::TriState]      = Direction::Bidir; // any direction for function
    directions[Function::SPI_CLK]       = Direction::Output;
    directions[Function::SPI_CS]        = Direction::Output;
    directions[Function::SPI_DATA_OUT]  = Direction::Output;
    directions[Function::SPI_DATA_IN]   = Direction::Input;

    return directions;
}

bool DevicePin::isAvaliableFunction(Function function) const
{
    return (std::find(m_avaliableFunctions.begin(), m_avaliableFunctions.end(), function) != m_avaliableFunctions.end());
}

const QString &DevicePin::name() const
{
    return m_name;
}

DevicePin::Direction DevicePin::direction() const
{
    return m_direction;
}

DevicePin::Function DevicePin::currentFunction() const
{
    return m_currentFunction;
}

const std::vector<DevicePin::Function> &DevicePin::avaliableFunctions() const
{
    return m_avaliableFunctions;
}

const std::vector<DevicePin::Direction> &DevicePin::avaliableDirections() const
{
    return m_avaliableDirections;
}
