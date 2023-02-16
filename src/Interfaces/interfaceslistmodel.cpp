#include <QDebug>

#include "interfaceslistmodel.h"

InterfacesListModel::InterfacesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

QHash<int, QByteArray> InterfacesListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::NameRole] = "name";
    roles[ListRoles::AvaliableRole] = "avaliable";
    roles[ListRoles::CurrentRole] = "current";
    return roles;
}

int InterfacesListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 0;
}

QVariant InterfacesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
        case ListRoles::NameRole:
        {
            return "";
        }

        case ListRoles::AvaliableRole:
        {
            return "";
        }

        case ListRoles::CurrentRole:
        {
            return "";
        }

        default:
        {
            qWarning() << __BASE_FILE__ << __FUNCTION__ << "no role";
            return true;
        }
    }
}

void InterfacesListModel::interfacesUpdated()
{
    //emit dataChanged(createIndex(0, 0), createIndex(m_data->size(), 0));
}
