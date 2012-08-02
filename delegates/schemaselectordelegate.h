#ifndef SCHEMASELECTORDELEGATE_H
#define SCHEMASELECTORDELEGATE_H

#include "comboboxdelegate.h"

class IQueryScheduler;

class SchemaSelectorDelegate : public ComboBoxDelegate
{
    Q_OBJECT
public:
    explicit SchemaSelectorDelegate(const QString &schemaName, IQueryScheduler *queryScheduler, QObject *parent, bool appendRowIfLast=false);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void setDefaultSchemaName(const QString &schemaName){this->schemaName=schemaName;}

private:
    QString schemaName;
    IQueryScheduler *queryScheduler;
    bool appendRowIfLast;
};

#endif // SCHEMASELECTORDELEGATE_H
