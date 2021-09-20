#ifndef REGISTERLISTMODEL_H
#define REGISTERLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QDebug>

#include "dutdevice.h"
#include "registeradapter.h"
#include "register.h"

class RegisterListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RegisterListModel(std::vector<std::shared_ptr<Register> > &registerList, QObject *parent = nullptr);
    RegisterListModel(std::vector<RegisterAdapter> &adapterList, QObject *parent = nullptr);
    RegisterListModel(QObject *parent = nullptr) : QAbstractListModel{parent} {};

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void resetModel();
    void resetModel(std::vector<std::shared_ptr<Register> > &registerList);

    Q_INVOKABLE void addItem(RegisterAdapter item, qint16 index);
    Q_INVOKABLE void changeItem(RegisterAdapter item, qint16 index);
    Q_INVOKABLE void removeItem(qint16 index);
    Q_INVOKABLE bool moveItem(qint16 sourceIndex, qint16 destIndex);

    Q_INVOKABLE RegisterAdapter getItem(qint16 index);

    std::vector<RegisterAdapter>& registerAdaptersList();
private:
    std::vector<RegisterAdapter> m_data;

    enum ListRoles{
        RegisterAdapterRole= Qt::UserRole + 1,
        isLocalRole
    };
};

#endif // REGISTERLISTMODEL_H
