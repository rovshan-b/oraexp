#include "checkablestandarditemmodel.h"

CheckableStandardItemModel::CheckableStandardItemModel(int rows, int columns, QObject *parent) :
    QStandardItemModel(rows, columns, parent)
{
}

bool CheckableStandardItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = QStandardItemModel::setData(index, value, role);

    if(role == Qt::CheckStateRole){
        QStandardItem *item = itemFromIndex(index);
        for( int i = 0; i < item->rowCount() ; i++ )
        {
            QStandardItem *childItem = item->child(i);
            if(childItem->checkState() != item->checkState()){
                childItem->setData(value, role);
            }
        }

        if(item->parent()){
            updateCheckedState(item->parent());
        }
    }

    return result;
}

void CheckableStandardItemModel::checkAll(bool check)
{
    QStandardItem *rootItem = invisibleRootItem();
    for(int i=0; i<rootItem->rowCount(); ++i){
        QStandardItem *childItem = rootItem->child(i);
        //childItem->setCheckState(check ? );
        setData(indexFromItem(childItem), check ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }
}

void CheckableStandardItemModel::checkAll()
{
    checkAll(true);
}

void CheckableStandardItemModel::uncheckAll()
{
    checkAll(false);
}

void CheckableStandardItemModel::updateCheckedState(QStandardItem *item)
{
    bool hasChecked = false;
    bool hasUnchecked = false;

    for( int i = 0; i < item->rowCount() ; i++ )
    {
        QStandardItem *childItem = item->child(i);
        if(childItem->checkState() == Qt::Checked){
            hasChecked = true;
        }else if(childItem->checkState() == Qt::Unchecked){
            hasUnchecked = true;
        }
    }

    Qt::CheckState newState;

    if(hasChecked && hasUnchecked){
        newState = Qt::PartiallyChecked;
    }else if(hasChecked && !hasUnchecked){
        newState = Qt::Checked;
    }else{
        newState = Qt::Unchecked;
    }

    item->setCheckState(newState);
}
