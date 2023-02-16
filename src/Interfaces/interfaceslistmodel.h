#ifndef INTERFACESLISTMODEL_H
#define INTERFACESLISTMODEL_H

#include <QAbstractListModel>

class InterfacesListModel : public QAbstractListModel
{
public:
    explicit InterfacesListModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;

public slots:
    void interfacesUpdated();

private:
    enum ListRoles{
        NameRole = Qt::UserRole + 1,
        AvaliableRole,
        CurrentRole
    };
};

#endif // INTERFACESLISTMODEL_H
