#include "dbitemlistdelegate.h"
#include "connectivity/dbconnection.h"
#include "widgets/dbitemlistcombobox.h"
#include <QtGui>

DbItemListDelegate::DbItemListDelegate(const QString &initialValue,
                                       IQueryScheduler *queryScheduler,
                                       const QString &queryName,
                                       const QString &iconName,
                                       QObject *parent,
                                       bool appendRowIfLast) :
    ComboBoxDelegate(parent),
    initialValue(initialValue),
    queryScheduler(queryScheduler),
    queryName(queryName),
    iconName(iconName),
    appendRowIfLast(appendRowIfLast)
{
}

QWidget *DbItemListDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    DbItemListComboBox *editor=new DbItemListComboBox(this->initialValue, iconName, true, true, parent);
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
