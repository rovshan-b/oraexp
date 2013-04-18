#include "dbitemlistdelegate.h"
#include "connectivity/dbconnection.h"
#include "widgets/dbitemlistcombobox.h"
#include "util/iconutil.h"
#include <QtGui>

DbItemListDelegate::DbItemListDelegate(const QString &initialValue,
                                       IQueryScheduler *queryScheduler,
                                       const QString &queryName,
                                       const QString &iconName,
                                       QObject *parent,
                                       bool appendRowIfLast) :
    ComboBoxDelegate(parent, -1, true),
    initialValue(initialValue),
    queryScheduler(queryScheduler),
    queryName(queryName),
    iconName(iconName),
    appendRowIfLast(appendRowIfLast),
    iconColumn(-1)
{
}

QWidget *DbItemListDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    DbItemListComboBox *editor=new DbItemListComboBox(this->initialValue, iconName, true, true, parent);
    if(iconColumn!=-1){
        editor->setIconColumn(iconColumn);
    }
    editor->setInDelegateMode();
    editor->loadItems(queryScheduler, queryName);
    editor->setFrame(false);

    return editor;
}


void DbItemListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const
{
    ComboBoxDelegate::setModelData(editor, model, index);
    QComboBox *comboBox = static_cast<QComboBox*>(editor);

    if(appendRowIfLast && !comboBox->currentText().trimmed().isEmpty()){
        int modelRowCount=model->rowCount();
        if(modelRowCount-1==index.row()){
            model->insertRows(modelRowCount, 1);
        }
    }
}

void DbItemListDelegate::setIconColumn(int colNum)
{
    this->iconColumn=colNum;
}
