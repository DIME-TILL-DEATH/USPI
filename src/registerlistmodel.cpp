#include "registerlistmodel.h"

RegisterListModel::RegisterListModel(std::vector<Register> &registerList, QObject *parent)
    : QAbstractListModel(parent)
{
    for(auto it = registerList.begin(); it != registerList.end(); ++it)
    {
        m_data.push_back(RegisterAdapter(&(*it)));
    }
}

RegisterListModel::RegisterListModel(std::vector<RegisterAdapter> &adapterList, QObject *parent)
    : QAbstractListModel(parent)
{
    m_data = adapterList;
}

QHash<int, QByteArray> RegisterListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::RegisterAdapterRole] = "register";
    roles[ListRoles::isLocalRole] = "isLocal";
    return roles;
}

int RegisterListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(m_data.size());
}

QVariant RegisterListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
        case ListRoles::RegisterAdapterRole:
        {
            RegisterAdapter adapter_ptr = m_data.at(index.row());
            Register* reg = adapter_ptr.getRegister();
            QByteArray result = reg->rawData();

            return QVariant::fromValue(m_data.at(index.row()));
        }
        case ListRoles::isLocalRole:
        {
            return m_data.at(index.row()).isLocal();
        }
        default:
        {
            return true;
        }
    }
}

bool RegisterListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count-1);
    m_data.insert(m_data.begin()+row, count, RegisterAdapter());
    endInsertRows();
    return true;
}

bool RegisterListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    m_data.erase(m_data.begin()+row, m_data.begin()+row+count);
    endRemoveRows();

    return true;
}

void RegisterListModel::resetModel()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void RegisterListModel::resetModel(std::vector<Register> &registerList)
{
    beginResetModel();
    m_data.clear();
    for(auto it = registerList.begin(); it != registerList.end(); ++it)
    {
        m_data.push_back(RegisterAdapter(&(*it)));
    }
    endResetModel();
}

void RegisterListModel::addItem(RegisterAdapter item, quint16 index)
{
    insertRows(index, 1);
    m_data.at(index) = item;
    emit dataChanged(createIndex(0, 0), createIndex(m_data.size(), 0));
}

void RegisterListModel::changeItem(RegisterAdapter item, quint16 index)
{
    m_data.at(index) = item;
    emit dataChanged(createIndex(0, 0), createIndex(m_data.size(), 0));
}

void RegisterListModel::removeItem(quint16 index)
{
    removeRows(index, 1);

    emit dataChanged(createIndex(0, 0), createIndex(m_data.size(), 0));
}

RegisterAdapter RegisterListModel::getItem(quint16 index)
{
    return m_data.at(index);
}

std::vector<RegisterAdapter> &RegisterListModel::registerAdaptersList()
{
    return m_data;
}
