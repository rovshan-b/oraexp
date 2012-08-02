#ifndef TABLESELECTORDELEGATE_H
#define TABLESELECTORDELEGATE_H

#include "comboboxdelegate.h"

class IQueryScheduler;

class TableSelectorDelegate : public ComboBoxDelegate
{
    Q_OBJECT
public:
    explicit TableSelectorDelegate(const QString &defaultSchemaName,
                                   int modelSchemaNameColumnIndex,
                                   IQueryScheduler *queryScheduler, QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const;

private:
    QString defaultSchemaName;
    int modelSchemaNameColumnIndex;
    IQueryScheduler *queryScheduler;

};

#endif // TABLESELECTORDELEGATE_H
