#include "partitionsmodel.h"

PartitionsModel::PartitionsModel(const QStringList &headerTitles, OraExp::PartitionPart configureFor, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent), configureFor(configureFor)
{
    if(configureFor==OraExp::PartitionPartPartition){
        PartitionNameColIx=0;
        SubpartitionNameColIx=-1;
        ValuesColIx=1;
        TablespaceColIx=2;
    }else if(configureFor==OraExp::PartitionPartSubpartition){
        PartitionNameColIx=0;
        SubpartitionNameColIx=1;
        ValuesColIx=2;
        TablespaceColIx=3;
    }else{
        PartitionNameColIx=-1;
        SubpartitionNameColIx=0;
        ValuesColIx=1;
        TablespaceColIx=2;
    }
}

QString PartitionsModel::isRowDataCorrect(int rowIx) const
{
    bool valuesColEnabled=(currentPartitionType!=OraExp::PartitionTypeHash);
    if(PartitionNameColIx!=-1 && !valuesColEnabled){
        QString partitionName=index(rowIx, PartitionNameColIx).data().toString().trimmed().toUpper();
        if(partitionName.isEmpty()){
            return tr("Partition name must be entered");
        }
    }

    if(PartitionNameColIx==-1 && SubpartitionNameColIx!=-1 && !valuesColEnabled){
        QString subpartitionName=index(rowIx, SubpartitionNameColIx).data().toString().trimmed().toUpper();
        if(subpartitionName.isEmpty()){
            return tr("Subpartition name must be entered");
        }
    }

    QString highValue=index(rowIx, ValuesColIx).data().toString().trimmed();

    if(highValue.isEmpty() && valuesColEnabled){
        return QString("\"%1\" must be entered").arg(headerData(ValuesColIx, Qt::Horizontal).toString());
    }

    return "";
}

PartitionInfo PartitionsModel::itemInfoFromModelRow(int row) const
{
    PartitionInfo info;
    info.partitionPosition=0;

    if(PartitionNameColIx!=-1){
        info.partitionName=index(row, PartitionNameColIx).data().toString().trimmed();
    }
    if(SubpartitionNameColIx!=-1){
        info.subpartitionName=index(row, SubpartitionNameColIx).data().toString().trimmed();
    }

    bool valuesColEnabled=(currentPartitionType!=OraExp::PartitionTypeHash);

    if(PartitionNameColIx!=-1 && info.partitionName.isEmpty() && !valuesColEnabled){
        info.partitionPosition=-1;
    }

    if(PartitionNameColIx==-1 && SubpartitionNameColIx!=-1 && info.subpartitionName.isEmpty() && !valuesColEnabled){
        info.partitionPosition=-1;
    }

    info.highValue=index(row, ValuesColIx).data().toString().trimmed();

    if(info.highValue.isEmpty() && valuesColEnabled && currentPartitionType!=OraExp::PartitionTypeHash){
        info.partitionPosition=-1;
    }

    info.tablespaceName=index(row, TablespaceColIx).data().toString().trimmed();

    if(info.partitionPosition!=-1){
        info.partitionPosition=row+1;

        info.markedForDeletion=isRowDeleted(row);
    }

    return info;
}
