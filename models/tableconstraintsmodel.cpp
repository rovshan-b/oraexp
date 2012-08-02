#include "tableconstraintsmodel.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include <QtGui>

TableConstraintsModel::TableConstraintsModel(const QStringList &headerTitles,
                                                     QObject *parent) :
    GenericEditableTableModel(headerTitles, parent), tableType(OraExp::TableTypeHeap)
{

}

Qt::ItemFlags TableConstraintsModel::flags ( const QModelIndex & index ) const
{
    Qt::ItemFlags itemFlags=GenericEditableTableModel::flags(index);

    if(itemFlags==Qt::NoItemFlags){
        return itemFlags;
    }

    int column=index.column();

    if((tableType==OraExp::TableTypeTemporaryTransaction || tableType==OraExp::TableTypeTemporarySession) &&
            column!=ConstraintConstraintType){
        OraExp::ConstraintType constraintType=getContraintType(index.row());
        if(constraintType==OraExp::ForeignKey){
            return Qt::NoItemFlags;
        }
    }


    if(column==ConstraintReferencedSchema ||
            column==ConstraintReferencedTable ||
            column==ConstraintReferencedColumn ||
            column==ConstraintOnDeleteAction) { //foreign key specific columns
        //get constraint type
        QVariant typeValue=this->index(index.row(), ConstraintConstraintType).data(Qt::EditRole);
        if(!typeValue.isValid()){ //if constraint type is not entered
            return Qt::NoItemFlags; //disable cell
        }

        int constraintType=typeValue.toInt();
        if(constraintType!=OraExp::ForeignKey) {
            return Qt::NoItemFlags;
        }
    }

    return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
}

QVariant TableConstraintsModel::data ( const QModelIndex & index, int role) const
{
    int column=index.column();

    if((tableType==OraExp::TableTypeTemporaryTransaction || tableType==OraExp::TableTypeTemporarySession) &&
            column!=ConstraintConstraintType &&
            role==Qt::BackgroundRole){
        OraExp::ConstraintType constraintType=getContraintType(index.row());
        if(constraintType==OraExp::ForeignKey){
            return QApplication::palette().background();
        }
    }

    if(column==ConstraintReferencedSchema && role==Qt::DecorationRole
            && GenericEditableTableModel::data(index, Qt::DisplayRole).isValid()){
        return IconUtil::getIcon("user");
    }else if(column==ConstraintReferencedTable && role==Qt::DecorationRole
            && GenericEditableTableModel::data(index, Qt::DisplayRole).isValid()){
        return IconUtil::getIcon("table");
    }else if(column==ConstraintReferencedColumn && role==Qt::DecorationRole
            && GenericEditableTableModel::data(index, Qt::DisplayRole).isValid()){
        return IconUtil::getIcon("column_uq");
    }

    if(role==Qt::BackgroundRole &&
            (column==ConstraintReferencedSchema ||
            column==ConstraintReferencedTable ||
            column==ConstraintReferencedColumn ||
            column==ConstraintOnDeleteAction)) { //foreign key specific columns
        //get constraint type
        OraExp::ConstraintType constraintType=getContraintType(index.row());

        if(constraintType!=OraExp::ForeignKey) {
            return QApplication::palette().background();
        }
    }

    QVariant value = GenericEditableTableModel::data(index, role);
    if(column==ConstraintConstraintType){ //constrant type

        if(!value.isNull() && role==Qt::DisplayRole)
        {
            int constraintType=value.toInt();
            value=DbUtil::getConstraintTypeName(constraintType);

        }else if(role==Qt::DecorationRole){
            QVariant displayValue=GenericEditableTableModel::data(index, Qt::DisplayRole);
            if(!displayValue.isValid()){
                return value;
            }
            int constraintType=displayValue.toInt();
            value=getKeyTypeIcon(constraintType);
        }

    }else if(column==ConstraintOnDeleteAction &&
            !value.isNull() &&
            role==Qt::DisplayRole){ //delete action
        int onDeleteAction=value.toInt();
        value=getOnDeleteActionName(onDeleteAction);
    }

    return value;
}

void TableConstraintsModel::setTableType(OraExp::TableType tableType)
{
    this->tableType=tableType;
}

OraExp::TableType TableConstraintsModel::getTableType() const
{
    return tableType;
}

QVariant TableConstraintsModel::getKeyTypeIcon(int constraintType) const
{
    QVariant value;
    switch(constraintType)
    {
    case 0:
        value=IconUtil::getIcon("column_pk");
        break;
    case 1:
        value=IconUtil::getIcon("column_fk");
        break;
    default:
        value=IconUtil::getIcon("column_uq");
        break;
    }
    return value;
}

QVariant TableConstraintsModel::getOnDeleteActionName(int onDeleteAction) const
{
    QVariant value;
    switch(onDeleteAction)
    {
    case 0:
        value=tr("No action");
        break;
    case 1:
        value=tr("Cascade");
        break;
    default:
        value=tr("Set null");
        break;
    }
    return value;
}

OraExp::ConstraintType TableConstraintsModel::getContraintType(int rowIx) const
{
    QVariant typeValue=this->index(rowIx, ConstraintConstraintType).data(Qt::EditRole);

    if(!typeValue.isValid()){
        return OraExp::UnknownConstraintType;
    }

    return (OraExp::ConstraintType)typeValue.toInt();
}

QString TableConstraintsModel::isRowDataCorrect(int rowIx) const
{
    Q_ASSERT(rowIx>=0 && rowIx<rowCount());

    QString consName=index(rowIx, TableConstraintsModel::ConstraintName).data().toString().trimmed();
    QVariant consType=index(rowIx, TableConstraintsModel::ConstraintConstraintType).data(Qt::EditRole);
    QString consColumns=index(rowIx, TableConstraintsModel::ConstraintColumns).data().toString().trimmed();

    if(consName.isEmpty() || !consType.isValid() || consColumns.isEmpty()){
        return tr("Constraint name, constraint type and constraint columns must be enetered");
    }
    OraExp::ConstraintType type=(OraExp::ConstraintType)consType.toInt();

    if(type==OraExp::ForeignKey){
        QString fkSchema=index(rowIx, TableConstraintsModel::ConstraintReferencedSchema).data().toString().trimmed();
        QString fkTable=index(rowIx, TableConstraintsModel::ConstraintReferencedTable).data().toString().trimmed();
        QString fkColumns=index(rowIx, TableConstraintsModel::ConstraintReferencedColumn).data().toString().trimmed();

        if(fkSchema.isEmpty() || fkTable.isEmpty() || fkColumns.isEmpty()){
            return tr("Schema name, referenced table and key columns must be entered for a foreign key constraint");
        }
    }

    return "";
}

ConstraintInfo TableConstraintsModel::itemInfoFromModelRow(int row) const
{
    ConstraintInfo consInfo;
    consInfo.constraintId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return consInfo;
    }

    consInfo.name=index(row, TableConstraintsModel::ConstraintName).data().toString().trimmed();
    consInfo.type=(OraExp::ConstraintType)index(row, TableConstraintsModel::ConstraintConstraintType).data(Qt::EditRole).toInt();
    consInfo.columns=index(row, TableConstraintsModel::ConstraintColumns).data().toString().trimmed();

    if(consInfo.type==OraExp::ForeignKey){
        consInfo.rOwner=index(row, TableConstraintsModel::ConstraintReferencedSchema).data().toString().trimmed();
        consInfo.rTableName=index(row, TableConstraintsModel::ConstraintReferencedTable).data().toString().trimmed();
        consInfo.rColumns=index(row, TableConstraintsModel::ConstraintReferencedColumn).data().toString().trimmed();
        consInfo.deleteRule=index(row, TableConstraintsModel::ConstraintOnDeleteAction).data(Qt::EditRole).toInt();
    }

    consInfo.deferrable=index(row, TableConstraintsModel::ConstraintDeferrable).data(Qt::EditRole).toBool();
    consInfo.deferred=index(row, TableConstraintsModel::ConstraintDeferred).data(Qt::EditRole).toBool();

    consInfo.constraintId=row+1;

    consInfo.markedForDeletion=isRowDeleted(row);

    return consInfo;
}
