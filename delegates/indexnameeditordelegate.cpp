#include "indexnameeditordelegate.h"

IndexNameEditorDelegate::IndexNameEditorDelegate(const QString &defaultSchemaName, QObject *parent) :
    IdentifierNameDelegate(parent),
    defaultSchemaName(defaultSchemaName)
{
}

void IndexNameEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    IdentifierNameDelegate::setModelData(editor, model, index);

    QModelIndex schemaIndex=index.model()->index(index.row(), 0);
    QVariant schemaData=schemaIndex.data();
    if(!schemaData.isValid() || schemaData.isNull()){
       model->setData(schemaIndex, this->defaultSchemaName);
    }
}
