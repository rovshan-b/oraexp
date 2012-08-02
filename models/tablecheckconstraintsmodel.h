#ifndef TABLECHECKCONSTRAINTSMODEL_H
#define TABLECHECKCONSTRAINTSMODEL_H

#include "genericeditabletablemodel.h"
#include "beans/constraintinfo.h"

class TableCheckConstraintsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit TableCheckConstraintsModel(const QStringList &headerTitles, QObject *parent);

    enum ColumnOrder
    {
        ConstraintName,
        ConstraintCondition,
        ConstraintDeferrable,
        ConstraintDeferred
    };

    virtual QString isRowDataCorrect(int rowIx) const;
    ConstraintInfo itemInfoFromModelRow(int row) const;

};

#endif // TABLECHECKCONSTRAINTSMODEL_H
