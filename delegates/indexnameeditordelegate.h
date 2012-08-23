#ifndef INDEXNAMEEDITORDELEGATE_H
#define INDEXNAMEEDITORDELEGATE_H

#include "identifiernamedelegate.h"

class IndexNameEditorDelegate : public IdentifierNameDelegate
{
    Q_OBJECT
public:
    explicit IndexNameEditorDelegate(const QString &defaultSchemaName, QObject *parent);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    QString defaultSchemaName;

};

#endif // INDEXNAMEEDITORDELEGATE_H
