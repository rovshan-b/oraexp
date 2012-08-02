#include "schemaselectordelegate.h"
#include "connectivity/dbconnection.h"
#include "widgets/dbitemlistcombobox.h"
#include <QtGui>

SchemaSelectorDelegate::SchemaSelectorDelegate(const QString &schemaName,
                                               IQueryScheduler *queryScheduler, QObject *parent, bool appendRowIfLast) :
    ComboBoxDelegate(parent), schemaName(schemaName), queryScheduler(queryScheduler), appendRowIfLast(appendRowIfLast)
{
}

QWidget *SchemaSelectorDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    DbItemListComboBox *editor=new DbItemListComboBox(this->schemaName, "user", true, true, parent);
    editor->loadItems(queryScheduler, "get_schema_list");
    editor->setFrame(false);

     return editor;
}


void SchemaSelectorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
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
