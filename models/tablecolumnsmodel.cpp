#include "tablecolumnsmodel.h"
#include "util/dbutil.h"
#include <QtGui>

TableColumnsModel::TableColumnsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent), tableType(OraExp::TableTypeHeap)
{
}

Qt::ItemFlags TableColumnsModel::flags ( const QModelIndex & index ) const
{
    Qt::ItemFlags itemFlags=GenericEditableTableModel::flags(index);

    if(itemFlags==Qt::NoItemFlags){
        return itemFlags;
    }

    int columnIx=index.column();

    if(columnIx==ColumnLobProperties) {
        //check if column type is LOB
        if(!isLob(index.row())){
            return Qt::NoItemFlags;
        }

        if(tableType==OraExp::TableTypeTemporaryTransaction || tableType==OraExp::TableTypeTemporarySession){
            return Qt::NoItemFlags;
        }
    }else if(columnIx==ColumnVirtualDefinition){
        int columnType=this->index(index.row(), ColumnColumnType).data(Qt::EditRole).toInt();
        if(columnType==OraExp::ColumnTypeNormal){
            return Qt::NoItemFlags;
        }
    }else if(columnIx==ColumnDefault){
        int columnType=this->index(index.row(), ColumnColumnType).data(Qt::EditRole).toInt();
        if(columnType!=OraExp::ColumnTypeNormal){
            return Qt::NoItemFlags;
        }
    }

    if(tableType==OraExp::TableTypeExternal &&
            (columnIx>=ColumnNotNull || columnIx==ColumnColumnType)){
        return Qt::NoItemFlags;
    }

    if(tableType!=OraExp::TableTypeHeap &&
            (columnIx==ColumnColumnType || columnIx==ColumnVirtualDefinition)){
        return Qt::NoItemFlags;
    }

    return (itemFlags | Qt::ItemIsEditable);
}

QVariant TableColumnsModel::data ( const QModelIndex & index, int role) const
{
    int columnIx=index.column();

    if(columnIx==ColumnLobProperties && role==Qt::BackgroundRole){
        if(!isLob(index.row())){
            return QApplication::palette().background();
        }

        if(tableType==OraExp::TableTypeTemporaryTransaction || tableType==OraExp::TableTypeTemporarySession){
            return QApplication::palette().background();
        }
    }/*else if(columnIx==ColumnColumnType && role==Qt::DisplayRole){
        int columnType=GenericEditableTableModel::data(index, role).toInt();
        return getColumnTypeName(columnType);
    }*/else if(columnIx==ColumnVirtualDefinition && role==Qt::BackgroundRole){
        int columnType=this->index(index.row(), ColumnColumnType).data(Qt::EditRole).toInt();
        if(columnType==OraExp::ColumnTypeNormal){
            return QApplication::palette().background();
        }
    }else if(columnIx==ColumnDefault && role==Qt::BackgroundRole){
        int columnType=this->index(index.row(), ColumnColumnType).data(Qt::EditRole).toInt();
        if(columnType!=OraExp::ColumnTypeNormal){
            return QApplication::palette().background();
        }
    }

    if(tableType==OraExp::TableTypeExternal && role==Qt::BackgroundRole && (columnIx>=ColumnNotNull || columnIx==ColumnColumnType)){
        return QApplication::palette().background();
    }

    if(tableType!=OraExp::TableTypeHeap &&
            role==Qt::BackgroundRole &&
            (columnIx==ColumnColumnType || columnIx==ColumnVirtualDefinition)){
        return QApplication::palette().background();
    }

    return GenericEditableTableModel::data(index, role);
}

bool TableColumnsModel::isLob(int rowIx) const
{
    QString dataType=this->index(rowIx, ColumnDataType).data().toString();
    return DbUtil::isLob(dataType);
}

void TableColumnsModel::setTableType(OraExp::TableType tableType)
{
    this->tableType=tableType;
}

OraExp::TableType TableColumnsModel::getTableType() const
{
    return tableType;
}

/*
QVariant TableColumnsModel::getColumnTypeName(int columnType) const
{
    QVariant value;
    switch(columnType)
    {
    case 0:
        value=tr("Normal");
        break;
    default:
        value=tr("Virtual");
        break;
    }

    return value;
}*/

QString TableColumnsModel::isRowDataCorrect(int rowIx) const
{
    Q_ASSERT(rowIx>=0 && rowIx<rowCount());

    QString colName=index(rowIx, ColumnName).data().toString().trimmed();
    QString dataType=index(rowIx, ColumnDataType).data().toString().trimmed();
    OraExp::ColumnType columnType=(OraExp::ColumnType)index(rowIx, ColumnColumnType).data(Qt::EditRole).toInt();
    QString virtualColumnDefinition=index(rowIx, ColumnVirtualDefinition).data().toString().trimmed();

    if(colName.isEmpty()){
        return tr("Column name must be entered");
    }

    if(columnType==OraExp::ColumnTypeNormal && dataType.isEmpty()){
        return tr("Data type of column must be entered");
    }

    if(columnType==OraExp::ColumnTypeVirtual && virtualColumnDefinition.isEmpty()){
        return tr("Virtual column definition must be entered");
    }

    return "";
}

ColumnInfo TableColumnsModel::itemInfoFromModelRow(int row) const
{
    ColumnInfo colInfo;
    colInfo.columnId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return colInfo;
    }

    colInfo.name=index(row, TableColumnsModel::ColumnName).data().toString().trimmed();
    colInfo.dataType=index(row, TableColumnsModel::ColumnDataType).data().toString().trimmed();
    colInfo.columnType=(OraExp::ColumnType)index(row, TableColumnsModel::ColumnColumnType).data(Qt::EditRole).toInt();
    colInfo.virtualDefinition=index(row, TableColumnsModel::ColumnVirtualDefinition).data().toString().trimmed();

    //OraExp::TableType tableType=getTableType();
    //if(tableType!=OraExp::TableTypeExternal){
        QVariant notNull=index(row, TableColumnsModel::ColumnNotNull).data(Qt::EditRole);
        colInfo.isNullable=notNull.isValid() ? !notNull.toBool() : true;
        colInfo.defaultValue=index(row, TableColumnsModel::ColumnDefault).data().toString();
        QVariant primaryKey=index(row, TableColumnsModel::ColumnPrimaryKey).data(Qt::EditRole);
        colInfo.isPrimaryKey=primaryKey.isValid() ? primaryKey.toBool() : false;
        QVariant unique=index(row, TableColumnsModel::ColumnUnique).data(Qt::EditRole);
        colInfo.isUnique=unique.isValid() ? unique.toBool() : false;
    //}

    QVariant params=index(row, TableColumnsModel::ColumnLobProperties).data(Qt::UserRole);
    if(params.isValid() && params.canConvert<LobParams>()){
        colInfo.lobParams=params.value<LobParams>();
    }else{
        colInfo.lobParams.isEmpty=true;
    }

    colInfo.comments=index(row, TableColumnsModel::ColumnComments).data().toString().trimmed();

    colInfo.columnId=row+1;

    colInfo.markedForDeletion=isRowDeleted(row);

    return colInfo;
}
