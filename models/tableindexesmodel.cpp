#include "tableindexesmodel.h"
#include "util/iconutil.h"
#include "beans/storageparams.h"
#include "beans/tablepartitioninginfo.h"
#include "util/dbutil.h"
#include <QtGui>

TableIndexesModel::TableIndexesModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
    setTitleColumn(IndexName);
}

Qt::ItemFlags TableIndexesModel::flags ( const QModelIndex & index ) const
{
    Qt::ItemFlags itemFlags=GenericEditableTableModel::flags(index);

    if(itemFlags==Qt::NoItemFlags){
        return itemFlags;
    }

    int column=index.column();

    if(column==IndexPrefixLength) {
        //check if compression is enabled
        int indexType=this->index(index.row(), IndexType).data(Qt::EditRole).toInt();
        bool compressionEnabled=this->index(index.row(), IndexCompress).data(Qt::EditRole).toBool();
        if(!compressionEnabled || indexType==OraExp::BitmapIndex){
            return Qt::NoItemFlags;
        }
    }else if(column==IndexCompress){
        int indexType=this->index(index.row(), IndexType).data(Qt::EditRole).toInt();
        if(indexType==OraExp::BitmapIndex){
            return Qt::NoItemFlags;
        }
    }else if(column==IndexParallelDegree){
        int parallelEnabled=this->index(index.row(), IndexParallel).data(Qt::EditRole).toInt();
        if(parallelEnabled!=1){
            return Qt::NoItemFlags;
        }
    }else if(column==IndexPartitionDefinition){
        int partitioningType=this->index(index.row(), TableIndexesModel::IndexPartitioningType).data(Qt::UserRole+1).toInt();
        if(partitioningType!=2){
            return Qt::NoItemFlags;
        }
    }

    return (itemFlags | Qt::ItemIsEditable);
}

QVariant TableIndexesModel::data ( const QModelIndex & index, int role) const
{
    int column=index.column();
    bool dataIsValid=GenericEditableTableModel::data(index, Qt::DisplayRole).isValid();

    QVariant value=GenericEditableTableModel::data(index, role);

    if(column==IndexOwner && role==Qt::DecorationRole && dataIsValid){
        return IconUtil::getIcon("user");
    }else if(column==IndexType && dataIsValid){
        if(role==Qt::DecorationRole){
            return IconUtil::getIcon("index");
        }else if(role==Qt::DisplayRole){
            return DbUtil::getIndexTypeName(value.toInt());
        }
    }else if(column==IndexPrefixLength && role==Qt::BackgroundRole){
        int indexType=this->index(index.row(), IndexType).data(Qt::EditRole).toInt();
        bool compressionEnabled=this->index(index.row(), IndexCompress).data(Qt::EditRole).toBool();
        if(!compressionEnabled || indexType==OraExp::BitmapIndex){
            return QApplication::palette().background();
        }
    }else if(column==IndexCompress && role==Qt::BackgroundRole){
        int indexType=this->index(index.row(), IndexType).data(Qt::EditRole).toInt();
        if(indexType==OraExp::BitmapIndex){
            return QApplication::palette().background();
        }
    }else if(column==IndexParallel && role==Qt::DisplayRole){
        return getParallelEnableText(value.toInt());
    }else if(column==IndexParallelDegree && role==Qt::BackgroundRole){
        int parallelEnabled=this->index(index.row(), IndexParallel).data(Qt::EditRole).toInt();
        if(parallelEnabled!=1){
            return QApplication::palette().background();
        }
    }else if(column==IndexPartitioningType && role==Qt::DisplayRole && value.isNull()){
        return tr("None");
    }else if(column==IndexPartitionDefinition && role==Qt::BackgroundRole){
        int partitioningType=this->index(index.row(), TableIndexesModel::IndexPartitioningType).data(Qt::UserRole+1).toInt();
        if(partitioningType!=2){
            return QApplication::palette().background();
        }
    }

    return value;
}

QVariant TableIndexesModel::getParallelEnableText(int parallelEnabledIx) const
{
    QVariant value;
    switch(parallelEnabledIx)
    {
    case 0:
        value="";
        break;
    case 1:
        value=tr("Enabled");
        break;
    default:
        value=tr("Disabled");
        break;
    }

    return value;
}

QString TableIndexesModel::isRowDataCorrect(int rowIx) const
{
    QString indexSchema=index(rowIx, TableIndexesModel::IndexOwner).data().toString().trimmed().toUpper();
    QString indexName=index(rowIx, TableIndexesModel::IndexName).data().toString().trimmed().toUpper();
    QString indexColumns=index(rowIx, TableIndexesModel::IndexColumns).data().toString().trimmed().toUpper();

    if(indexSchema.isEmpty() || indexName.isEmpty() || indexColumns.isEmpty()){
        return tr("Index name and columns must be entered");
    }

    return "";
}

IndexInfo TableIndexesModel::itemInfoFromModelRow(int row) const
{
    IndexInfo ixInfo;
    ixInfo.indexId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return ixInfo;
    }

    bool conversionOk;

    ixInfo.owner=index(row, TableIndexesModel::IndexOwner).data().toString().trimmed().toUpper();
    ixInfo.name=index(row, TableIndexesModel::IndexName).data().toString().trimmed().toUpper();
    ixInfo.columns=index(row, TableIndexesModel::IndexColumns).data().toString().trimmed().toUpper();

    ixInfo.type=(OraExp::IndexType)index(row, TableIndexesModel::IndexType).data(Qt::EditRole).toInt();
    ixInfo.compress=index(row, TableIndexesModel::IndexCompress).data(Qt::EditRole).toBool();
    ixInfo.prefixLength=index(row, TableIndexesModel::IndexPrefixLength).data().toInt(&conversionOk);
    if(!conversionOk){
        ixInfo.prefixLength=-1;
    }

    ixInfo.parallel=index(row, TableIndexesModel::IndexParallel).data(Qt::EditRole).toInt()==1 ? 1 : 2;
    ixInfo.parallelDegree=index(row, TableIndexesModel::IndexParallelDegree).data().toInt(&conversionOk);
    if(!conversionOk){
        ixInfo.parallelDegree=-1;
    }

    ixInfo.reverse=index(row, TableIndexesModel::IndexReverse).data(Qt::EditRole).toBool();

    QVariant storage=index(row, TableIndexesModel::IndexStorage).data(Qt::UserRole);
    if(storage.isValid() && storage.canConvert<StorageParams>()){
        ixInfo.storageParams=storage.value<StorageParams>();
        ixInfo.storageParams.isEmpty=false;
    }else{
        ixInfo.storageParams.isEmpty=true;
    }

    ixInfo.partitioning=index(row, TableIndexesModel::IndexPartitioningType).data(Qt::UserRole+1).toInt();

    if(ixInfo.partitioning==2){
        QVariant params=index(row, TableIndexesModel::IndexPartitionDefinition).data(Qt::UserRole);
        if(params.isValid() && params.canConvert<TablePartitioningInfo>()){
            ixInfo.partInfo=params.value<TablePartitioningInfo>();
            //ixInfo.partInfo.isEmpty=false;
        }else{
            ixInfo.partInfo.isEmpty=true;
        }
    }

    ixInfo.generated=false;

    ixInfo.indexId=row+1;

    ixInfo.markedForDeletion=isRowDeleted(row);

    return ixInfo;
}
