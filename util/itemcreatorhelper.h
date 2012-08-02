#ifndef ITEMCREATORHELPER_H
#define ITEMCREATORHELPER_H

#include <QList>
#include "strutil.h"
#include "models/genericeditabletablemodel.h"
#include "widgets/datatable.h"

class DataTable;

class ItemCreatorHelper
{
public:
    ItemCreatorHelper();

    template <typename T>
    static void moveRowAfterCreationError(DataTable *table, const QString &taskName, QList<T> *originalItemList)
    {
        int rowNum=numberAfterLastUnderscore(taskName);
        Q_ASSERT(rowNum>0);

        GenericEditableTableModel *model=static_cast<GenericEditableTableModel*>(table->model());
        int rowIx=rowNum-1;

        table->setUpdatesEnabled(false);

        model->addCopyOf(rowIx);
        model->removeRow(rowIx);
        originalItemList->removeAt(rowIx);
        model->freezeRow(model->getFrozenRow()-1);

        table->setUpdatesEnabled(true);
    }

    template <typename ItemType, typename ModelType>
    static void handleTableDataChanged(ModelType *model,
                                QList<ItemType> *originalItemList,
                                const QModelIndex &from,
                                const QModelIndex &to)
    {
        if(model->getFrozenRow()<0){
            return;
        }

        int startRow=from.row();
        int endRow=to.row();
        for(int i=startRow; i<=endRow; ++i){
            if(model->isRowFrozen(i) && !model->isRowDeleted(i)){
                markDataChanges(model, i, originalItemList);
            }
        }
    }

    template <typename ItemType, typename ModelType>
    static void markDataChanges(ModelType *model, int rowIx,
                         QList<ItemType> *originalItemList)
    {
        Q_ASSERT(rowIx>=0 && rowIx<originalItemList->size());

        ItemType currentItemInfo=model->itemInfoFromModelRow(rowIx);
        ItemType originalItemInfo=originalItemList->at(rowIx);

        bool hasChanges=!(currentItemInfo==originalItemInfo);
        if(model->isRowChanged(rowIx)!=hasChanges){
            model->setRowChanged(rowIx, hasChanges);
        }

        bool needsRecreation=currentItemInfo.needsRecreation(originalItemInfo);
        if(model->rowNeedsRecreation(rowIx)!=needsRecreation){
            model->setRowNeedsRecreation(rowIx, needsRecreation);
        }
    }
};

#endif // ITEMCREATORHELPER_H
