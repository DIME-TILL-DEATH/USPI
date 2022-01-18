#ifndef DEVICEPIN_H
#define DEVICEPIN_H

#include <vector>
#include <QString>
#include <QMap>

class DevicePin
{
public:
    enum class Direction{
        Input,
        Output,
        Bidir
    };
    enum class Function{
        TriState = 0,
        SPI_DATA_IN,
        SPI_DATA_OUT,
        SPI_CLK,
        SPI_CS
    };

    DevicePin(const QString& name="unknown",
              const std::vector<Direction>& avaliableDirections={Direction::Input, Direction::Output, Direction::Bidir},
              const std::vector<Function>& avaliableFunctions={Function::TriState});

    static QMap<Function, Direction> pinDirForFunction();

    bool isAvaliableFunction(Function function) const;

    const QString &name() const;
    Direction direction() const;
    Function currentFunction() const;

    const std::vector<Function> &avaliableFunctions() const;
    const std::vector<Direction> &avaliableDirections() const;

private:
    QString m_name;
    Direction m_direction{Direction::Bidir};
    Function m_currentFunction{Function::TriState};

    std::vector<Direction> m_avaliableDirections;
    std::vector<Function> m_avaliableFunctions;
};

#endif // DEVICEPIN_H
