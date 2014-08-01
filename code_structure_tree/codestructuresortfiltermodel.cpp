#include "codestructuresortfiltermodel.h"
#include "codestructuretreeitem.h"

CodeStructureSortFilterModel::CodeStructureSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    //setDynamicSortFilter(true);
}

bool CodeStructureSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if(!left.isValid() || !right.isValid()){
        return false;
    }

    CodeStructureTreeItem *leftItem = static_cast<CodeStructureTreeItem *>(left.internalPointer());
    CodeStructureTreeItem *rightItem = static_cast<CodeStructureTreeItem *>(right.internalPointer());

    Q_ASSERT(leftItem);
    Q_ASSERT(rightItem);

    if(!left.parent().isValid() &&
            !right.parent().isValid()){
        return codeStructureTreeItemNameLessThan(leftItem, rightItem);
    }else{
        return codeStructureTreeItemLessThan(leftItem, rightItem);
    }
}
