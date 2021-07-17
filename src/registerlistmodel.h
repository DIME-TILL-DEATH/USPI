#ifndef REGISTERLISTMODEL_H
#define REGISTERLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "dutdevice.h"
#include "registeradapter.h"
#include "register.h"

class RegisterListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RegisterListModel(std::vector<Register> &registerList, QObject *parent = nullptr);
    RegisterListModel(std::vector<RegisterAdapter> &adapterList, QObject *parent = nullptr);
    RegisterListModel(QObject *parent = nullptr) : QAbstractListModel{parent} {};

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


    void resetModel(std::vector<Register> &registerList);

    Q_INVOKABLE void addItem(RegisterAdapter item, quint16 index);
    Q_INVOKABLE void removeItem(quint16 index);
    Q_INVOKABLE RegisterAdapter getItem(quint16 index);

    std::vector<RegisterAdapter>& registerAdaptersList();
private:
    std::vector<RegisterAdapter> m_data;

    enum ListRoles{
        RegisterAdapterRole= Qt::UserRole + 1,
    };
};

#endif // REGISTERLISTMODEL_H
