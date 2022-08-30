#ifndef DUTLISTMODEL_H
#define DUTLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include <QDebug>

#include "dutdevice.h"
#include "registeradapter.h"
#include "register.h"

class DutListModel : public QAbstractListModel
{
public:
    explicit DutListModel(std::vector<std::shared_ptr<DUTDevice> >* data, QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void addDutToList(DUTDevice* newDevice);
    void removeDutFromList(quint16 index);

public slots:
    void dutUpdated();
    void refreshModel();

private:
    std::vector<std::shared_ptr<DUTDevice> >* m_data;

    enum ListRoles{
        DeviceHeaderRole = Qt::UserRole + 1
    };
};

#endif // DUTLISTMODEL_H
