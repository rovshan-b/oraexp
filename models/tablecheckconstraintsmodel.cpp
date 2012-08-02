#include "tablecheckconstraintsmodel.h"

TableCheckConstraintsModel::TableCheckConstraintsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
}

QString TableCheckConstraintsModel::isRowDataCorrect(int rowIx) const
{
    QString consName=index(rowIx, ConstraintName).data().toString().trimmed();
    QString consCondition=index(rowIx, ConstraintCondition).data().toString().trimmed();

    if(consName.isEmpty() || consCondition.isEmpty()){
        return tr("Constraint name and condition must be entered");
    }

    return "";
}

ConstraintInfo TableCheckConstraintsModel::itemInfoFromModelRow(int row) const
{
    ConstraintInfo consInfo;
    consInfo.constraintId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return consInfo;
    }

    consInfo.name=index(row, ConstraintName).data().toString().trimmed();
    consInfo.type=OraExp::CheckConstraint;
    consInfo.searchCondition=index(row, ConstraintCondition).data().toString().trimmed();
    consInfo.deferrable=index(row, ConstraintDeferrable).data(Qt::EditRole).toBool();
    consInfo.deferred=index(row, ConstraintDeferred).data(Qt::EditRole).toBool();

    consInfo.constraintId=row+1;

    consInfo.markedForDeletion=isRowDeleted(row);

    return consInfo;
}
