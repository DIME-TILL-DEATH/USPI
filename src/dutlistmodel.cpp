#include "dutlistmodel.h"
#include "dutdevice.h"

DutListModel::DutListModel(std::vector<std::shared_ptr<DUTDevice> > *data, QObject *parent)
    : QAbstractListModel{parent},
      m_data{data}
{

}

QHash<int, QByteArray> DutListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::DeviceHeaderRole] = "deviceHeader";
    return roles;
}

int DutListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(m_data->size());
}

QVariant DutListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
        case ListRoles::DeviceHeaderRole:
        {
            return QVariant::fromValue(m_data->at(index.row())->deviceHeader());
        }

        default:
        {
            qWarning() << __BASE_FILE__ << __FUNCTION__ << "no role";
            return true;
        }
    }
}

bool DutListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count-1);
  //  m_data->insert(m_data->begin()+row, count, RegisterAdapter());
    endInsertRows();
    return true;
}

bool DutListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    m_data->erase(m_data->begin()+row, m_data->begin()+row+count);
    endRemoveRows();
    return true;
}

void DutListModel::addDutToList(DUTDevice* newDevice)
{
    insertRows(m_data->size(),1);
    m_data->push_back(std::shared_ptr<DUTDevice>(newDevice));

    emit dataChanged(createIndex(0, 0), createIndex(m_data->size(), 0));
}

void DutListModel::removeDutFromList(quint16 index)
{
    removeRows(index, 1);
    emit dataChanged(createIndex(0, 0), createIndex(m_data->size(), 0));
}

std::shared_ptr<DUTDevice> DutListModel::getDeviceByIndex(quint16 index)
{
    if(index > m_data->size())
    {
            qWarning() << __FUNCTION__ << ": index is bigger than device list size";
            return {};
    }

    return m_data->at(index);
}

void DutListModel::dutUpdated()
{
    emit dataChanged(createIndex(0, 0), createIndex(m_data->size(), 0));
}

void DutListModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}
