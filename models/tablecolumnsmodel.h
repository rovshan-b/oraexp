#ifndef TABLECOLUMNSMODEL_H
#define TABLECOLUMNSMODEL_H

#include "genericeditabletablemodel.h"
#include "enums.h"
#include "beans/columninfo.h"

class TableColumnsModel : public GenericEditableTableModel
{
public:
    TableColumnsModel(const QStringList &headerTitles, QObject *parent);

    enum ColumnOrder
    {
        ColumnName,
        ColumnColumnType,
        ColumnDataType,
        ColumnVirtualDefinition,
        ColumnNotNull,
        ColumnDefault,
        ColumnPrimaryKey,
        ColumnUnique,
        ColumnLobProperties,
        ColumnComments
    };

    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole) const;

    bool isLob(int rowIx) const;

    void setTableType(OraExp::TableType tableType);
    OraExp::TableType getTableType() const;

    virtual QString isRowDataCorrect(int rowIx) const;

    ColumnInfo itemInfoFromModelRow(int row) const;

private:
    OraExp::TableType tableType;
    QVariant getColumnTypeName(int columnType) const;
};

#endif // TABLECOLUMNSMODEL_H
