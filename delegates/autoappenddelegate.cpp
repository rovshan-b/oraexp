#include "autoappenddelegate.h"

AutoAppendDelegate::AutoAppendDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void AutoAppendDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
    QString value=index.data().toString();

     if(!value.isEmpty()){
         int modelRowCount=model->rowCount();
         if(modelRowCount-1==index.row()){
             model->insertRows(modelRowCount, 1);
         }
     }
}
