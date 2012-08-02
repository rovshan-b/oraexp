#include "tableindexesddlgenerator.h"
#include "util/strutil.h"

TableIndexesDdlGenerator::TableIndexesDdlGenerator()
{
}

QString TableIndexesDdlGenerator::generateDdl(const TableInfo &tableInfo, bool storage, bool tablespaceOnly)
{
    QString ddl;

    QString fullTableName=QString("\"%1\".\"%2\"").arg(tableInfo.generalInfo.schema).arg(tableInfo.generalInfo.tableName);

    int ddlCount=tableInfo.indexes.size();
    QString ddlLine;
    for(int i=0; i<ddlCount; ++i){
        ddlLine=QString("\n%1;").arg(tableInfo.indexes.at(i).generateDdl(fullTableName, tableInfo.generalInfo.tableType, storage, tablespaceOnly));
        ddl.append(ddlLine);
    }

    return ddl;
}

QList<NameQueryPair> TableIndexesDdlGenerator::generateAlterDdl(const TableInfo &tableInfo)
{
    QList< NameQueryPair > result;

    const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;

    OraExp::TableType tableType=generalInfo->tableType;

    QString schema=generalInfo->schema;
    QString tableName=generalInfo->tableName;
    QString fullTableName=QString("\"%1\".\"%2\"").arg(schema).arg(tableName);

    QList<IndexInfo> *originalIndexList=&tableInfo.originalInfo()->indexes;
    Q_ASSERT(originalIndexList->size()<=tableInfo.indexes.size());

    int rowCount=tableInfo.indexes.size();
    for(int i=0; i<rowCount; ++i){
        const IndexInfo &ixInfo=tableInfo.indexes.at(i);

        if(i<originalIndexList->size()){
            const IndexInfo &originalIxInfo=originalIndexList->at(i);
            QList< NameQueryPair > alterDdls=ixInfo.generateDiffDdl(originalIxInfo, schema, tableName, tableType);
            NameQueryPair ddlPair;
            for(int i=0; i<alterDdls.size(); ++i){
                ddlPair=alterDdls.at(i);
                result.append(qMakePair(ddlPair.first, ddlPair.second));
            }
        }else{
            result.append(qMakePair(QString("add_index_%1").arg(i+1), ixInfo.generateDdl(fullTableName, tableType)));
        }
    }

    return result;
}

QString TableIndexesDdlGenerator::generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, const QStringList &droppedColumnNames,
                                                  bool storageForNew, bool tablespaceOnlyForNew)
{
    QString ddl;

    QString fullTargetTableName=QString("\"%1\".\"%2\"").arg(targetTableInfo.generalInfo.schema).arg(targetTableInfo.generalInfo.tableName);

    bool found;

    for(int i=0; i<targetTableInfo.indexes.size(); ++i){
        const IndexInfo &targetIx=targetTableInfo.indexes.at(i);
        sourceTableInfo.findIndexByColumns(targetIx.columns, found);
        if(found){
            continue;
        }

        //check if column for this index is dropped. if so this index will automatically be dropped
        if(existsInCollection(targetIx.columns, droppedColumnNames)){
            continue;
        }

        if(!ddl.isEmpty()){
            ddl.append("\n");
        }
        ddl.append(targetIx.generateDropDdl(targetTableInfo.generalInfo.schema)).append(";");
    }


    for(int i=0; i<sourceTableInfo.indexes.size(); ++i){
        const IndexInfo &sourceIx=sourceTableInfo.indexes.at(i);
        const IndexInfo &targetIx=targetTableInfo.findIndexByColumns(sourceIx.columns, found);

        if(found){ //check if we need to alter it
            if(existsInCollection(targetIx.columns, droppedColumnNames)){ //index will be dropped together with column, so create as new
                addEOL(ddl);
                ddl.append(sourceIx.generateDdl(fullTargetTableName, targetTableInfo.generalInfo.tableType)).append(";");
            }else{ //alter
                QList<NameQueryPair> ddlList=sourceIx.generateDiffDdl(targetIx,
                                                                      targetTableInfo.generalInfo.schema,
                                                                      targetTableInfo.generalInfo.tableName,
                                                                      targetTableInfo.generalInfo.tableType,
                                                                      false);
                foreach(NameQueryPair pair, ddlList){
                    if(!ddl.isEmpty()){
                        ddl.append("\n");
                    }
                    ddl.append(pair.second).append(";");
                }
            }

        }else{ //add it
            addEOL(ddl);
            ddl.append(sourceIx.generateDdl(fullTargetTableName, targetTableInfo.generalInfo.tableType, storageForNew, tablespaceOnlyForNew)).append(";");
        }
    }

    return ddl;
}
