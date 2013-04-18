#ifndef CONSTRAINTSELECTORDELEGATE_H
#define CONSTRAINTSELECTORDELEGATE_H

#include "comboboxdelegate.h"
#include "enums.h"

class IQueryScheduler;

class ConstraintSelectorDelegate : public ComboBoxDelegate
{   
    Q_OBJECT
public:
    explicit ConstraintSelectorDelegate(const QString &defaultSchemaName,
                                   int modelSchemaNameColumnIndex,
                                   int modelTableNameColumnIndex,
                                   OraExp::ConstraintType constraintType,
                                   const QString &initialColumnName,
                                   IQueryScheduler *queryScheduler,
                                   QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

private:
    QString defaultSchemaName;
    int modelSchemaNameColumnIndex;
    int modelTableNameColumnIndex;
    OraExp::ConstraintType constraintType;
    QString initialColumnName;
    IQueryScheduler *queryScheduler;
};

#endif // CONSTRAINTSELECTORDELEGATE_H
