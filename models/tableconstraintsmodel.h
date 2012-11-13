#ifndef TABLECONSTRAINTSMODEL_H
#define TABLECONSTRAINTSMODEL_H

#include "genericeditabletablemodel.h"
#include "enums.h"
#include "beans/constraintinfo.h"

class TableConstraintsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:

    enum ColumnNames {
        ConstraintName,
        ConstraintConstraintType,
        ConstraintColumns,
        ConstraintReferencedSchema,
        ConstraintReferencedTable,
        ConstraintReferencedColumn,
        ConstraintOnDeleteAction,
        ConstraintDeferrable,
        ConstraintDeferred
    };

    explicit TableConstraintsModel(
            const QStringList &headerTitles,
            QObject *parent);

    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    QVariant data ( const QModelIndex & index, int role) const;

    void setTableType(OraExp::TableType tableType);
    OraExp::TableType getTableType() const;

    virtual QString isRowDataCorrect(int rowIx) const;

    ConstraintInfo itemInfoFromModelRow(int row) const;

private:
    OraExp::ConstraintType getContraintType(int rowIx) const;

    OraExp::TableType tableType;
};

#endif // TABLECONSTRAINTSMODEL_H
