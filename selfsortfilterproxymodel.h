#ifndef SELFSORTFILTERPROXYMODEL_H
#define SELFSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QDate>
#include <QSortFilterProxyModel>

class SelfSortFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    //SelfSortFilterProxyModel(QObject *parent = nullptr);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
};

#endif // SELFSORTFILTERPROXYMODEL_H
