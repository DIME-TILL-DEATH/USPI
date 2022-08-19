#include <algorithm>
#include <vector>

#include "registerlistmodel.h"

RegisterListModel::RegisterListModel(std::vector<std::shared_ptr<Register> > &registerList, QObject *parent)
    : QAbstractListModel(parent)
{
    for(auto it = registerList.begin(); it != registerList.end(); ++it)
    {
        m_data.push_back(RegisterAdapter(*it));
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
    roles[ListRoles::ParentDUTHeaderRole] = "deviceHeader";
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
            return QVariant::fromValue(m_data.at(index.row()));
        }

        case ListRoles::ParentDUTHeaderRole:
        {
            return QVariant::fromValue(m_data.at(index.row()).parentDUTHeader());
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

void RegisterListModel::resetModel(const std::vector<std::shared_ptr<Register> > &registerList)
{
    beginResetModel();
    m_data.clear();
    for(auto it = registerList.begin(); it != registerList.end(); ++it)
    {
        m_data.push_back(RegisterAdapter(*it));
    }
    endResetModel();
}

void RegisterListModel::addItem(RegisterAdapter item, qint16 index)
{
    if(index<0) index=0; // when no items in list, index=-1

    insertRows(index, 1);
    m_data.at(index) = item;
    emit dataChanged(createIndex(0, 0), createIndex(m_data.size(), 0));
}

void RegisterListModel::changeItem(RegisterAdapter item, qint16 index)
{
    m_data.at(index) = item;
    emit dataChanged(createIndex(0, 0), createIndex(m_data.size(), 0));
}

void RegisterListModel::removeItem(qint16 index)
{
    removeRows(index, 1);
    emit dataChanged(createIndex(0, 0), createIndex(m_data.size(), 0));
}

bool RegisterListModel::moveItem(qint16 sourceIndex, qint16 destIndex)
{
    if(sourceIndex < 0 || sourceIndex > static_cast<qint16>(m_data.size())) return false;
    if(destIndex < 0 || destIndex > static_cast<qint16>(m_data.size())) return false;
    if(destIndex == sourceIndex) return false;

    if(destIndex > sourceIndex)
     {
        beginMoveRows(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), destIndex+1);
        std::rotate(m_data.begin()+sourceIndex, m_data.begin()+sourceIndex+1, m_data.begin()+destIndex+1);
     }
     else if(destIndex < sourceIndex)
     {
        beginMoveRows(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), destIndex);
        std::rotate(m_data.rbegin()+(m_data.size()-sourceIndex), m_data.rbegin()+(m_data.size()-sourceIndex-1), m_data.rbegin()+(m_data.size()-destIndex-1));
     }
    endMoveRows();
    return true;
}

RegisterAdapter RegisterListModel::getItem(qint16 index)
{
    return m_data.at(index);
}

RegisterAdapter *RegisterListModel::getRegisterAdapterByUniqueId(quint16 uniqueId)
{
    for(auto it = m_data.begin(); it != m_data.end(); ++it)
    {
        if((*it).getRegister()->uniqueId() == uniqueId) return &(*it);
    }
    return nullptr;
}

std::vector<RegisterAdapter> &RegisterListModel::registerAdaptersList()
{
    return m_data;
}
