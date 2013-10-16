#include "grantobjectlistdelegate.h"
#include "widgets/dbitemlistcombobox.h"

GrantObjectListDelegate::GrantObjectListDelegate(IQueryScheduler *queryScheduler, int modelSchemaColumnIndex, QObject *parent) :
    DbItemListDelegate("",queryScheduler,"get_object_list_for_grant",
                       "",parent,true),
    modelSchemaColumnIndex(modelSchemaColumnIndex)
{
}

QWidget *GrantObjectListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    DbItemListComboBox *editor=new DbItemListComboBox(this->initialValue, iconName, true, true, parent);
    editor->setInDelegateMode();
    editor->setIconColumn(2);
    editor->setFrame(false);

    QModelIndex schemaIndex=index.model()->index(index.row(), modelSchemaColumnIndex);
    QString schemaName=schemaIndex.data().toString().trimmed().toUpper();

    if(!schemaName.isEmpty()){
        editor->loadItems(queryScheduler, queryName,
                          QList<Param*>() << new Param(":owner", schemaName));
    }

    return editor;
}
