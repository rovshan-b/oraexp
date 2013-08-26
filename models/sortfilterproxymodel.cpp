#include "sortfilterproxymodel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

QVariant SortFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation==Qt::Vertical){
        return section+1;
    }else{
        return QSortFilterProxyModel::headerData(section, orientation, role);
    }
}
