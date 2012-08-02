#include "tablecolumnsddlgenerator.h"
#include "util/dbutil.h"
#include "util/strutil.h"

TableColumnsDdlGenerator::TableColumnsDdlGenerator()
{
}

QString TableColumnsDdlGenerator::generateDdl(const TableInfo &tableInfo, bool includeInlinePkConstraint)
{
    const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;

    QString ddl;

    int ddlCount=tableInfo.columns.size();
    for(int i=0; i<ddlCount; ++i){
        ddl.append("\t").append(tableInfo.columns.at(i).generateDdl(generalInfo->tableType, includeInlinePkConstraint));

        if(i!=ddlCount-1){
            ddl.append(",\n");
        }
    }

    return ddl;
}

QList<NameQueryPair> TableColumnsDdlGenerator::generateAlterDdl(const TableInfo &tableInfo)
{
    QList< NameQueryPair > result;

    const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;

    OraExp::TableType tableType=generalInfo->tableType;

    QString fullTableName=QString("\"%1\".\"%2\"").arg(generalInfo->schema).arg(generalInfo->tableName);

    QList<ColumnInfo> *originalColumnList=&tableInfo.originalInfo()->columns;
    Q_ASSERT(originalColumnList->size()<=tableInfo.columns.size());

    int rowCount=tableInfo.columns.size();

    for(int i=0; i<rowCount; ++i){
        const ColumnInfo &colInfo=tableInfo.columns.at(i);

        if(i<originalColumnList->size()){ //frozen row in table
            const ColumnInfo &originalColInfo=originalColumnList->at(i);
            QList< NameQueryPair > alterDdls=colInfo.generateDiffDdl(originalColInfo, fullTableName, tableType);
            NameQueryPair ddlPair;
            for(int i=0; i<alterDdls.size(); ++i){
                ddlPair=alterDdls.at(i);
                result.append(qMakePair(ddlPair.first, ddlPair.second));
            }
        }else{
            result.append(qMakePair(QString("add_table_column_%1").arg(i+1), TableColumnsDdlGenerator::generateAppendDdl(tableInfo, i, true)));

            if(!colInfo.comments.isEmpty()){
                result.append(qMakePair(QString("add_column_comments_%1").arg(i+1), TableColumnsDdlGenerator::generateCommentsDdlForRow(tableInfo, fullTableName, i)));
            }
        }
    }

    return result;
}

QString TableColumnsDdlGenerator::generateAppendDdl(const TableInfo &tableInfo,
                                                    int rowIx,
                                                    bool includeInlinePkUqConstraints, bool lobParams, bool storage, bool tablespaceOnly)
{
    QString fullTableName=QString("\"%1\".\"%2\"").arg(tableInfo.generalInfo.schema).arg(tableInfo.generalInfo.tableName);

    QString appendColumnDdl;
    appendColumnDdl.append(QString("ALTER TABLE %1 ADD %2").arg(fullTableName, tableInfo.columns.at(rowIx).generateDdl(tableInfo.generalInfo.tableType, includeInlinePkUqConstraints)));

    if(lobParams && DbUtil::isLob(tableInfo.columns.at(rowIx).dataType)){
        appendColumnDdl.append(TableColumnsDdlGenerator::generateLobParamsDdlForRow(tableInfo, rowIx, storage, tablespaceOnly));
    }

    return appendColumnDdl;
}

QString TableColumnsDdlGenerator::generateLobParamsDdl(const TableInfo &tableInfo)
{
    int rowCount=tableInfo.columns.size();

    QString ddl;
    QString rowDdl;

    for(int i=0; i<rowCount; ++i){
        if(!DbUtil::isLob(tableInfo.columns.at(i).dataType)){
            continue;
        }

        rowDdl=generateLobParamsDdlForRow(tableInfo, i);
        if(!rowDdl.isEmpty()){
            ddl.append(rowDdl);
        }
    }

    return ddl;
}

QString TableColumnsDdlGenerator::generateLobParamsDdlForRow(const TableInfo &tableInfo, int row, bool storage, bool tablespaceOnly)
{
    QString ddl;

    Q_ASSERT(row<tableInfo.columns.size());

    const ColumnInfo &colInfo=tableInfo.columns.at(row);

    QString colName=colInfo.name;

    QString lobParams;
    TableInfo *originalTableInfo=tableInfo.originalInfo();
    if(originalTableInfo && originalTableInfo->columns.size()>row && DbUtil::isLob(originalTableInfo->columns.at(row).dataType)){
        lobParams = colInfo.lobParams.generateDiffDdl(tableInfo.originalInfo()->columns.at(row).lobParams);
    }else{
        lobParams = colInfo.lobParams.generateDdl(storage, tablespaceOnly);
    }

    if(colName.isEmpty() || lobParams.isEmpty()){
        return "";
    }

    ddl.append("\nLOB (\"").append(colName).append("\") STORE AS ").append(lobParams);

    return ddl;
}

QString TableColumnsDdlGenerator::generateCommentsDdl(const TableInfo &tableInfo)
{
    QString fullTableName=QString("\"%1\".\"%2\"").arg(tableInfo.generalInfo.schema).arg(tableInfo.generalInfo.tableName);

    QString ddl;
    int rowCount=tableInfo.columns.size();

    for(int i=0; i<rowCount; ++i){

        if(tableInfo.columns.at(i).comments.isEmpty()){
            continue;
        }

        ddl.append("\n").append(TableColumnsDdlGenerator::generateCommentsDdlForRow(tableInfo, fullTableName, i)).append(";");
    }

    return ddl;
}

QString TableColumnsDdlGenerator::generateCommentsDdlForRow(const TableInfo &tableInfo, const QString &fullTableName, int row)
{
    Q_ASSERT(row<tableInfo.columns.size());

    QString ddl;

    const ColumnInfo &colInfo=tableInfo.columns.at(row);

    if(colInfo.comments.isEmpty()){
        return "";
    }

    QString commentsToEscape=colInfo.comments;

    ddl.append("COMMENT ON COLUMN ").append(fullTableName).
    append(".\"").append(colInfo.name).append("\" ").
            append("IS '").append(commentsToEscape.replace("'", "''")).append("'");

    return ddl;
}

QString TableColumnsDdlGenerator::generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo,
                                                  QStringList &droppedColumnNames, bool comments, bool lobParams, bool storageForNew, bool tablespaceOnlyForNew)
{
    QString ddl;

    QString fullTargetTableName=QString("\"%1\".\"%2\"").arg(targetTableInfo.generalInfo.schema).arg(targetTableInfo.generalInfo.tableName);


    bool found;

    for(int i=0; i<sourceTableInfo.columns.size(); ++i){
        const ColumnInfo &sourceCol=sourceTableInfo.columns.at(i);
        const ColumnInfo &targetCol=targetTableInfo.findColumnByName(sourceCol.name, found);

        if(found){ //check if we need to alter it
            QList<NameQueryPair> ddlList=sourceCol.generateDiffDdl(targetCol, fullTargetTableName, OraExp::TableTypeHeap, comments, lobParams);
            foreach(NameQueryPair pair, ddlList){
                addEOL(ddl);
                ddl.append(pair.second).append(";");
            }
        }else{ //add it
            addEOL(ddl);
            TableInfo fakeTableInfo=sourceTableInfo;
            fakeTableInfo.generalInfo.schema=targetTableInfo.generalInfo.schema;
            ddl.append(TableColumnsDdlGenerator::generateAppendDdl(fakeTableInfo, i, false, lobParams, storageForNew, tablespaceOnlyForNew)).append(";");

            if(!sourceCol.comments.isEmpty()){
                ddl.append("\n").append(TableColumnsDdlGenerator::generateCommentsDdlForRow(fakeTableInfo, fullTargetTableName, i)).append(";");
            }
        }
    }

    for(int i=0; i<targetTableInfo.columns.size(); ++i){
        const ColumnInfo &targetCol=targetTableInfo.columns.at(i);
        sourceTableInfo.findColumnByName(targetCol.name, found);
        if(found){
            continue;
        }

        addEOL(ddl);
        ddl.append(targetCol.generateDropDdl(fullTargetTableName)).append(";");
        droppedColumnNames.append(targetCol.name);
    }

    return ddl;
}
