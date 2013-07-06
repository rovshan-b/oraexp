#ifndef TREESORTFILTERPROXYMODEL_H
#define TREESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class TreeSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TreeSortFilterProxyModel(QObject *parent);

protected:
    bool filterAcceptsRow ( int sourceRow, const QModelIndex & sourceParent ) const;
};

#endif // TREESORTFILTERPROXYMODEL_H
