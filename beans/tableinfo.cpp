#include "tableinfo.h"
#include "util/dbutil.h"
#include "code_generators/ddl/tableddlgenerator.h"

TableInfo::TableInfo() : originalTableInfo(0)
{
}

TableInfo::~TableInfo()
{
    qDebug("deleted TableInfo");
}

void TableInfo::setSchemaName(const QString &newSchemaName)
{
    QString currentSchemaName=generalInfo.schema;

    generalInfo.schema=newSchemaName;

    for(int i=0; i<indexes.size(); ++i){
        if(indexes.at(i).owner==currentSchemaName){
            indexes[i].owner=newSchemaName;
        }
    }

    for(int i=0; i<constraints.size(); ++i){
        if(constraints.at(i).rOwner==currentSchemaName){
            constraints[i].rOwner=newSchemaName;
        }
    }

    for(int i=0; i<triggers.size(); ++i){
        TriggerInfo &triggerInfo=triggers[i];
        if(triggerInfo.owner==currentSchemaName){
            triggerInfo.owner=newSchemaName;
        }
        if(!triggerInfo.follows.isEmpty()){
            triggerInfo.follows.replace(QString("\"%1\".").arg(currentSchemaName), QString("\"%1\".").arg(newSchemaName));
        }
        if(!triggerInfo.precedes.isEmpty()){
            triggerInfo.follows.replace(QString("\"%1\".").arg(currentSchemaName), QString("\"%1\".").arg(newSchemaName));
        }
    }
}

void TableInfo::prepareForCreateLike(const TableDiffDdlOptions &options)
{
    if(!options.comments){
        generalInfo.comments = "";
    }

    if(!options.properties){
        generalInfo.additionalAttributes = TableAdditionalAttributes();
    }

    if(!options.flashbackArchive){
        generalInfo.additionalAttributes.flashbackArchive = 0;
        generalInfo.additionalAttributes.flashbackArchiveName = "";
    }

    if(!options.storage){
        generalInfo.storageParams = StorageParams();
        generalInfo.storageParams.isEmpty = true;

        if(options.iotProperties){
            generalInfo.indexOrganizedProperties.storageParams = StorageParams();
            generalInfo.indexOrganizedProperties.storageParams.isEmpty = true;
        }
    }

    if(!options.iotProperties){
        generalInfo.indexOrganizedProperties = IndexOrganizedTableProperties();
    }

    if(!options.columns){
        columns.clear();
    }

    if(!options.constraints){
        constraints.clear();
        checkConstraints.clear();
    }

    if(!options.indexes){
        indexes.clear();
    }

    if(!options.partitions){
        partitioningInfo = TablePartitioningInfo();
        partitioningInfo.isEmpty = true;
    }

    if(!options.externalProperties){
        externalInfo = TableExternalInfo();
    }

    if(!options.triggers){
        triggers.clear();
    }

    if(!options.grants){
        grants.clear();
    }

    for(int i=0; i<columns.size(); ++i){
        ColumnInfo &colInfo = columns[i];
        colInfo.isPrimaryKey = false; //will be created through Keys tab
        colInfo.isUnique = false;
        colInfo.lobParams.segment = ""; //must use different segment

        if(!options.comments){
            colInfo.comments = "";
        }

        if(!options.lobProperties){
            colInfo.lobParams = LobParams();
            colInfo.lobParams.isEmpty = true;
        }else if(!options.storage){
            colInfo.lobParams.storageParams = StorageParams();
            colInfo.lobParams.storageParams.isEmpty = true;
        }
    }

    if(options.indexes && (!options.storage || !options.partitions)){
        for(int i=0; i<indexes.size(); ++i){
            IndexInfo &indexInfo = indexes[i];
            if(!options.storage){
                indexInfo.storageParams = StorageParams();
                indexInfo.storageParams.isEmpty = true;
            }
            if(!options.partitions){
                indexInfo.partitioning = 0;
                indexInfo.partInfo = TablePartitioningInfo();
                indexInfo.partInfo.isEmpty = true;
            }
        }
    }
}

ColumnInfo TableInfo::findColumnByName(const QString &columnName, bool &found) const
{
    foreach(const ColumnInfo &colInfo, columns){
        if(colInfo.name==columnName){
            found=true;
            return colInfo;
        }
    }

    found=false;
    return ColumnInfo();
}

ConstraintInfo TableInfo::findConstraintByColumns(OraExp::ConstraintType constraintType, const QString &constraintColumns, bool &found) const
{
    if(constraintType!=OraExp::CheckConstraint){
        foreach(const ConstraintInfo &consInfo, this->constraints){
            if(consInfo.type==constraintType && consInfo.columns==constraintColumns){
                found=true;
                return consInfo;
            }
        }
    }

    if(constraintType==OraExp::CheckConstraint){
        foreach(const ConstraintInfo &consInfo, this->checkConstraints){
            if(consInfo.type==constraintType && consInfo.columns==constraintColumns){
                found=true;
                return consInfo;
            }
        }
    }

    found=false;
    return ConstraintInfo();
}

IndexInfo TableInfo::findIndexByColumns(const QString &indexColumns, bool &found) const
{
    foreach(const IndexInfo &indexInfo, indexes){
        if(indexInfo.columns==indexColumns){
            found=true;
            return indexInfo;
        }
    }

    found=false;
    return IndexInfo();
}

ObjectGrantInfo TableInfo::findGrantByGrantee(const QString &grantee, bool &found) const
{
    foreach(const ObjectGrantInfo &grantInfo, grants){
        if(grantInfo.grantee==grantee){
            found=true;
            return grantInfo;
        }
    }

    found=false;
    return ObjectGrantInfo();
}

TriggerInfo TableInfo::findTriggerByName(const QString &triggerName, bool &found) const
{
    foreach(const TriggerInfo &triggerInfo, triggers){
        if(triggerInfo.name==triggerName){
            found=true;
            return triggerInfo;
        }
    }

    found=false;
    return TriggerInfo();
}

void TableInfo::setLobTablespace(const QString &lobTablespace)
{
    for(int i=0; i<columns.size(); ++i){
        if(!DbUtil::isLob(columns.at(i).dataType)){
            continue;
        }

        columns[i].lobParams.storageParams.tablespaceName=lobTablespace;
    }
}

void TableInfo::prepareForOptions(const NewDbObjectStorageOptions &options)
{
    if(options.storage && !options.tablespaceName.isEmpty()){
        this->generalInfo.storageParams.tablespaceName=options.tablespaceName;
    }

    if(options.storage && !options.iotOverflowTablespaceName.isEmpty()){
        this->generalInfo.indexOrganizedProperties.storageParams.tablespaceName=options.iotOverflowTablespaceName;
    }

    if(options.storage && !options.indexTablespaceName.isEmpty()){
        for(int i=0; i<indexes.size(); ++i){
            indexes[i].storageParams.tablespaceName=options.indexTablespaceName;
            indexes[i].partInfo.setPartitionsTablespace(options.indexTablespaceName);
        }
    }else if(!options.storage){
        for(int i=0; i<indexes.size(); ++i){
            indexes[i].partInfo.setPartitionsTablespace("");
        }
    }

    if(options.storage && !options.lobTablespaceName.isEmpty()){
        setLobTablespace(options.lobTablespaceName);
    }else if(!options.storage){
        setLobTablespace("");
    }

    if(options.storage && !options.partitionsTablespaceName.isEmpty()){
        this->partitioningInfo.setPartitionsTablespace(options.partitionsTablespaceName);
    }else if(!options.storage){
        this->partitioningInfo.setPartitionsTablespace(options.partitionsTablespaceName);
    }
}

TableInfo *TableInfo::originalInfo() const
{
    return originalTableInfo;
}

void TableInfo::setOriginalInfo(TableInfo *originalTableInfo)
{
    this->originalTableInfo=originalTableInfo;
}

QString TableInfo::generateDdl(bool includeInlinePkConstraint,
                               bool includeConstraints,
                               QList<OraExp::ConstraintType> constraintTypes,
                               bool includeGrants, const TableCreateDdlOptions &options) const
{
    return TableDdlGenerator::generateDdl(*this, includeInlinePkConstraint, includeConstraints, constraintTypes, includeGrants, options);
}

QList<QueryListItem> TableInfo::generateAlterDdl(const QHash<QString, QObject *> &requesters) const
{
    return TableDdlGenerator::generateAlterDdl(*this, requesters);
}

void TableInfo::removeGeneratedNotNullConstraints()
{
    foreach(const ColumnInfo &colInfo, columns){
        if(!colInfo.isNullable){
            //try to find generated not null constraint on this column
            const QString searchCondition=QString("\"%1\" IS NOT NULL").arg(colInfo.name);
            for(int i=checkConstraints.size()-1; i>=0; --i){
                const ConstraintInfo &constraintInfo=checkConstraints.at(i);
                if(constraintInfo.searchCondition==searchCondition){
                    checkConstraints.removeAt(i);
                }
            }
        }
    }
}

bool TableInfo::needsRecreation(const TableInfo &other, bool checkPartitioning) const
{
    bool sameType = this->generalInfo.tableType == other.generalInfo.tableType;

    if(!sameType){
        return true;
    }

    if(!checkPartitioning){
        return false;
    }

    bool samePartitioning = this->partitioningInfo.isEmpty == other.partitioningInfo.isEmpty;
    if(sameType && samePartitioning && !this->partitioningInfo.isEmpty){
        samePartitioning = this->partitioningInfo.partitionInfo.partitioningStrategy == other.partitioningInfo.partitionInfo.partitioningStrategy;
        if(samePartitioning){
            samePartitioning = this->partitioningInfo.partitionInfo.type == other.partitioningInfo.partitionInfo.type;
            if(samePartitioning && this->partitioningInfo.partitionInfo.partitioningStrategy==1){
                samePartitioning = this->partitioningInfo.subpartitionInfo.type == other.partitioningInfo.subpartitionInfo.type;
            }
        }
    }

    return !samePartitioning;
}
