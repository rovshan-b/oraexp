#include "tableselectordelegate.h"
#include "connectivity/dbconnection.h"
#include "widgets/dbitemlistcombobox.h"
#include <QtGui>

TableSelectorDelegate::TableSelectorDelegate(const QString &defaultSchemaName,
                                             int modelSchemaNameColumnIndex,
                                             IQueryScheduler *queryScheduler, QObject *parent) :
    ComboBoxDelegate(parent),
    defaultSchemaName(defaultSchemaName),
    modelSchemaNameColumnIndex(modelSchemaNameColumnIndex),
    queryScheduler(queryScheduler)
{
}

QWidget *TableSelectorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &index) const
{
    QString schemaName=this->defaultSchemaName;
    if(modelSchemaNameColumnIndex!=1){
        QModelIndex schemaIndex=index.model()->index(index.row(), modelSchemaNameColumnIndex);
        QString modelSchemaName=schemaIndex.data().toString();
        if(!modelSchemaName.trimmed().isEmpty()){
            schemaName=modelSchemaName;
        }
    }

    DbItemListComboBox *editor=new DbItemListComboBox(index.data().toString(), "table", true, true, parent);
    editor->setInDelegateMode();
    editor->loadItems(queryScheduler, "get_table_list", QList<Param*>() << new Param(":owner", schemaName) << new Param(":table_name", QString("")));
    editor->setFrame(false);

    return editor;
}

void TableSelectorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
     QComboBox *comboBox = static_cast<QComboBox*>(editor);

     QString value = comboBox->currentText().toUpper();

     model->setData(index, value, Qt::EditRole);

     if(modelSchemaNameColumnIndex!=-1){
         QModelIndex schemaIndex=index.model()->index(index.row(), modelSchemaNameColumnIndex);
         QVariant schemaData=schemaIndex.data();
         if(!schemaData.isValid() || schemaData.isNull()){
            model->setData(schemaIndex, this->defaultSchemaName);
         }
     }
}
