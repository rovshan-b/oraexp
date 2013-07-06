#include "treesortfilterproxymodel.h"

TreeSortFilterProxyModel::TreeSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool TreeSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QRegExp regExp = filterRegExp();
    if(regExp.isEmpty()){
        return true;
    }

    int colCount = sourceModel()->columnCount(sourceParent);

    for(int i=0; i<colCount; ++i){
        QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
        QString text = index.data().toString();
        if(text.contains(regExp)){
            return true;
        }else if(i==0 && sourceModel()->hasChildren(index)){
            for(int k=0; k<sourceModel()->rowCount(index); ++k){
                if(filterAcceptsRow(k, index)){
                    return true;
                }
            }
        }
    }

    return false;
}
