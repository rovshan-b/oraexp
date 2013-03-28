#include "datacomparerthread.h"
#include "util/queryutil.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "util/strutil.h"
#include <QDebug>
#include "util/dbutil.h"
#include "beans/datacomparisonoptions.h"
#include "defines.h"

DataComparerThread::DataComparerThread(const QString &sourceSchema,
                                       DbConnection *sourceDb,
                                       const QString &targetSchema,
                                       DbConnection *targetDb,
                                       const QString &tableName,
                                       DataOperationOptions *options,
                                       const TableInfoForDataComparison &tableOptions,
                                       QObject *parent) :
    DataOperationThread(sourceSchema, sourceDb, targetSchema, targetDb, tableName, options, tableOptions, parent),
    targetStmt(0),
    sourceDeleteGeneratorStmt(0)
{
    compareOptions = static_cast<DataComparisonOptions*>(options);

    compareScript=QueryUtil::getQuery("compare_data", targetDb);
    reverseCompareScript=QueryUtil::getQuery("compare_data_reverse", sourceDb);

    bulkHelper.setBulkSize(BULK_DATA_OPERATION_CHUNK_SIZE);
    bulkDeleteHelper.setBulkSize(BULK_DATA_OPERATION_CHUNK_SIZE);
}

DataComparerThread::~DataComparerThread()
{
    delete targetStmt;
    delete sourceDeleteGeneratorStmt;
}

void DataComparerThread::run()
{
    emit statusChanged(QString("Comparing data for table %1").arg(this->tableName));

    QString currentTaskName="compare_table_data";

    try{
        if(tableOptions.columnsToCompare.isEmpty()){ //no columns selected for comparison
            emitCompareInfo(tableName, tr("Skipped. No columns selected."));
            emitCompletedSignal();
            return;
        }

        QString uqCols=getUqColumns();
        this->uqColumns = splitTrimmed(uqCols);
        if(uqColumns.isEmpty()){ //no columns to use for uniquely identifying records
            emitCompareInfo(tableName, tr("Skipped. No PK or unique key selected."));
            emitCompletedSignal();
            return;
        }

        if(!allExistInCollection(uqCols, tableOptions.columnsToCompare)){
            emitCompareInfo(tableName, tr("Skipped. No complete PK or unique key selected."));
            emitCompletedSignal();
            return;
        }

        targetStmt=targetDb->createStatement();
        sourceDeleteGeneratorStmt=sourceDb->createStatement();

        currentTaskName="prepare_bind_arrays";
        prepareBindArrays();

        currentTaskName="create_comparison_script";
        createComparisonScript();

        if(compareOptions->deletes){
            currentTaskName="reverse_compare_data";
            doReverseComparison();
        }

        if(compareOptions->inserts || compareOptions->updates){
            currentTaskName="compare_data";
            doComparison();
        }

        emitCompareInfo(tableName, tr("Completed"));
        emitCompletedSignal();
    }catch(OciException ex){
        emit compareError(currentTaskName, ex);
    }
}

QString DataComparerThread::getUqColumns()
{
    QString result;

    QString query=QueryUtil::getQuery("get_table_constraint_columns");
    QueryResult res=sourceDb->executeQuery(query, QList<Param*>() <<
                                  new Param(":owner", sourceSchema) <<
                                  new Param(":object_name", tableName));
    QScopedPointer<Statement> stmt(res.statement);

    Resultset *rs=stmt->rsAt(0);

    rs->beginFetchRows();

    QString consColNames;
    while(rs->moveNext()){
        consColNames = rs->getString(1);
        if(allExistInCollection(consColNames, tableOptions.columnsToCompare)){
            result = consColNames;
            break;
        }//else one or more of constraint columns not selected in tree view
    }
    rs->endFetchRows();

    return result;
}

void DataComparerThread::prepareBindArrayForColumn(const QString &colName, const QString &dataType, int length, int colOffset)
{
    if(compareOptions->inserts || compareOptions->deletes){
        bulkHelper.createBufferForDataType(targetStmt, dataType, length);
    }

    if(compareOptions->deletes && uqColumns.contains(colName)){
        bulkDeleteHelper.createBufferForDataType(sourceDeleteGeneratorStmt, dataType, length);
        uqColumnOffsets.append(colOffset);
    }
}

void DataComparerThread::createComparisonScript()
{
    QString columnNames = joinEnclosed(tableOptions.columnsToCompare, ",", "\"");
    QString columnNamesForSelectInto = getColumnsForSelect(tableOptions.columnsToCompare);

    QStringList columnVariableDeclarations;
    QStringList columnVariableNames;

    QStringList uqColumnsANDCollAll;
    QStringList uqColumnsANDCollValues;
    QStringList allCollValues;

    QString updateScript;

    QString colVarDecl;
    QString colVarName;
    QString collName;

    QString dataType;

    for(int i=0; i<tableOptions.columnsToCompare.size(); ++i){

        QString currentColumnName=tableOptions.columnsToCompare.at(i);

        colVarName=QString("col_var_%1").arg(i);
        dataType=bulkHelper.dataTypeAt(i);
        int length = bulkHelper.lengthAt(i);

        colVarDecl=QString("%1 %2").arg(colVarName, dataType);
        if(length>0){
            colVarDecl.append("(").append(QString::number(length)).append(")");
        }
        colVarDecl.append(";");

        //do not use dataType for variable declarations after this point
#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
        dataType=allColumns.value(currentColumnName);
#endif


        collName=QString(":coll_%1(i)").arg(i);

        columnVariableNames.append(colVarName);
        columnVariableDeclarations.append(colVarDecl);

        allCollValues.append(toDynamicSqlValue(collName, dataType));

        if(!uqColumns.contains(currentColumnName)){
            updateScript.append("\t if (").append(collName).append(" is null and ").append(colVarName).append(" is not null) or\n");
            updateScript.append("\t\t  (").append(collName).append(" is not null and ").append(colVarName).append(" is null) or\n");
            updateScript.append("\t\t  (").append(collName).append(" != ").append(colVarName).append(") then");

            updateScript.append("\n\t\t\t if l_update_sql is not null then ").append(" l_update_sql := l_update_sql || ','; end if;");
            //QString printValue = QString("dbms_output.put_line('%1='||%1||', '||'%2='||%2);").arg(collName, colVarName);
            updateScript.append("\n\t\t\t ")/*.append(printValue)*/.append(" l_update_sql := l_update_sql || '\"").
                    append(currentColumnName).append("\" = ").
                    append(toDynamicSqlValue(collName, dataType)).
                    append("';");
            updateScript.append("\n\t end if;\n");
        }else{
#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
            uqColumnsANDCollAll.append(QString("\"%1\" = %2").arg(currentColumnName,
                                                              DbUtil::toIntervalOrTimestamp(collName, dataType)));
#else
            uqColumnsANDCollAll.append(QString("\"%1\" = %2").arg(currentColumnName, collName));
#endif

            uqColumnsANDCollValues.append(QString("\"%1\" = %2").arg(currentColumnName, toDynamicSqlValue(collName, dataType)));
        }
    }


    QString targetTableName = tableOptions.targetTableName.isEmpty() ? this->tableName : tableOptions.targetTableName;
    //this may be different if user disabled schema name generation in script
    QString scriptTargetTableName = (compareOptions->comparisonMode==DataComparisonOptions::GenerateDml && !compareOptions->includeSchemaName) ?
                                    QString("\"%1\"").arg(targetTableName) :
                                    QString("\"%1\".\"%2\"").arg(targetSchema, targetTableName);

    QString fullTargetTableName = QString("\"%1\".\"%2\"").arg(targetSchema, targetTableName);

    QString enclosedUqColumns=joinEnclosed(uqColumns, ",", "\"");

    QString joinedUqColumnsANDCollAll=uqColumnsANDCollAll.join(" and ");
    QString joinedUqColumnsANDCollValues=uqColumnsANDCollValues.join(" and ");

    compareScript.replace("{col_names}", columnNames);
    compareScript.replace("{col_names_for_select_into}", columnNamesForSelectInto);
    compareScript.replace("{col_var_declarations}", columnVariableDeclarations.join(" "));
    compareScript.replace("{col_var_names}", columnVariableNames.join(","));
    compareScript.replace("{target_table}", fullTargetTableName);
    compareScript.replace("{dyn_target_table}", scriptTargetTableName);
    compareScript.replace("{uq_columns_and_coll_all}", joinedUqColumnsANDCollAll);
    compareScript.replace("{uq_columns_and_coll_values}", joinedUqColumnsANDCollValues);
    compareScript.replace("{all_coll_values}", allCollValues.join("'||','||'"));
    compareScript.replace("{compare_col_values_script}", updateScript);

    QString sourceTableName = QString("\"%1\".\"%2\"").arg(sourceSchema, this->tableName);

    reverseCompareScript.replace("{source_table}", sourceTableName);
    reverseCompareScript.replace("{uq_columns}", enclosedUqColumns);
    reverseCompareScript.replace("{uq_columns_and_coll_all}", joinedUqColumnsANDCollAll);
    reverseCompareScript.replace("{uq_columns_and_coll_values}", joinedUqColumnsANDCollValues);
    reverseCompareScript.replace("{dyn_target_table}", scriptTargetTableName);

   qDebug() << "compare script:" << compareScript;
   //qDebug() << "reverse compare script:" << reverseCompareScript;
}

void DataComparerThread::doComparison()
{
    emitCompareInfo(tableName, tr("Generating INSERT/UPDATE statements"));

    QString selectFromSourceSql=QString("SELECT %1 FROM \"%2\".\"%3\"").
            arg(getColumnsForSelect(tableOptions.columnsToCompare)).
            arg(sourceSchema).arg(tableName);

    if(!tableOptions.whereClause.isEmpty()){
        selectFromSourceSql.append(ensureStartsWith(tableOptions.whereClause, "WHERE"));
    }

    qDebug() << "select statement from source table:" << selectFromSourceSql;


    QueryResult res=sourceDb->executeQuery(selectFromSourceSql);

    QScopedPointer<Statement> stmt(res.statement);

    Resultset *rs=stmt->rsAt(0);

    targetStmt->lockConnection();

    targetStmt->prepare(compareScript);
    //targetStmt->setBindArraySize(DATA_COMPARER_CHUNK_SIZE);

    int offset=0;

    Param *arrSizeParam=new Param(":arr_size", 0);
    Param *insertCountParam=new Param(":insert_count", 0);
    Param *updateCountParam=new Param(":update_count", 0);

    bulkHelper.bindArrays(targetStmt, "coll"); //first
    QList<Param*> params = QList<Param*>() <<
                                              arrSizeParam <<
                                              insertCountParam <<
                                              updateCountParam <<
                                              new Param(":generate_inserts", this->compareOptions->inserts) <<
                                              new Param(":generate_updates", this->compareOptions->updates) <<
                                              new Param(":update_database", (int)this->compareOptions->comparisonMode) <<
                                              new Param(":rs_out");
    targetStmt->bindParams(params); //second. do not move upper than first
    //targetStmt->printBindVars();

    rs->beginFetchRows();

    int fetchedCount=0;

    QString dml;

    while(rs->moveNext()){

        ++fetchedCount;

        for(int colIx=0; colIx<tableOptions.columnsToCompare.size(); ++colIx){
            bulkHelper.setArrayData(targetStmt, rs, colIx+1, offset);
        }

        ++offset;
        if(offset==BULK_DATA_OPERATION_CHUNK_SIZE){
            arrSizeParam->setIntValue(offset);
            targetStmt->execute();

            dml=targetStmt->rsAt(0)->fetchOneAsString(1);

            targetStmt->releaseResultsets();

            emitCompareInfo(tableName, "", insertCountParam->getIntValue(), updateCountParam->getIntValue(), 0, dml);

            emit statusChanged(QString("Comparing data for table %1 (%2 records done - INSERT/UPDATE)").arg(this->tableName).arg(fetchedCount));

            offset=0;

            if(this->stopped){
                break;
            }
        }
    }

    if(offset>0){
        arrSizeParam->setIntValue(offset);
        bulkHelper.nullifyArrayData(targetStmt, offset);
        targetStmt->execute();

        dml=targetStmt->rsAt(0)->fetchOneAsString(1);

        emitCompareInfo(tableName, "", insertCountParam->getIntValue(), updateCountParam->getIntValue(), 0, dml);
    }

    rs->endFetchRows();

    qDebug() << "insert count:" << insertCountParam->getIntValue() << ", update count:" << updateCountParam->getIntValue();

    targetStmt->unlockConnection();
}

void DataComparerThread::doReverseComparison()
{
    emitCompareInfo(tableName, tr("Generating DELETE statements"));

    QString selectFromTargetSql=QString("SELECT %1 FROM \"%2\".\"%3\"").
            arg(getColumnsForSelect(uqColumns)).
            arg(targetSchema).arg(tableOptions.targetTableName.isEmpty() ? this->tableName : tableOptions.targetTableName);

    if(!tableOptions.whereClause.isEmpty()){
        selectFromTargetSql.append(" ").append(ensureStartsWith(tableOptions.whereClause, "WHERE"));
    }

    qDebug() << selectFromTargetSql;

    QueryResult res=targetDb->executeQuery(selectFromTargetSql);
    QScopedPointer<Statement> stmt(res.statement);

    sourceDeleteGeneratorStmt->lockConnection();

    sourceDeleteGeneratorStmt->prepare(reverseCompareScript);

    Param *arrSizeParam=new Param(":arr_size", 0);
    Param *deleteCountParam=new Param(":delete_count", 0);

    bulkDeleteHelper.bindArrays(sourceDeleteGeneratorStmt, "coll", uqColumnOffsets); //first
    sourceDeleteGeneratorStmt->bindParams(QList<Param*>() <<
                                          arrSizeParam <<
                                          deleteCountParam <<
                                          new Param(":rs_out")); //second. do not move upper than first
    //sourceDeleteGeneratorStmt->printBindVars();

    int offset=0;

    Resultset *rs=stmt->rsAt(0);
    rs->beginFetchRows();

    int fetchedCount=0;

    QString dml;
    QString fullDml;

    int currentDeleteCount=0;
    while(rs->moveNext()){

        ++fetchedCount;

        for(int colIx=0; colIx<uqColumns.size(); ++colIx){
            bulkDeleteHelper.setArrayData(sourceDeleteGeneratorStmt, rs, colIx+1, offset);
        }

        ++offset;
        if(offset==BULK_DATA_OPERATION_CHUNK_SIZE){
            arrSizeParam->setIntValue(offset);
            sourceDeleteGeneratorStmt->execute();

            dml=sourceDeleteGeneratorStmt->rsAt(0)->fetchOneAsString(1);

            sourceDeleteGeneratorStmt->releaseResultsets();

            if(!dml.isEmpty()){
                if(compareOptions->comparisonMode==DataComparisonOptions::GenerateDml){
                    emitCompareInfo(tableName, "", 0, 0, deleteCountParam->getIntValue(), dml);
                }else{
                    emitCompareInfo(tableName, "", 0, 0, deleteCountParam->getIntValue(), "");
                    currentDeleteCount+=deleteCountParam->getIntValue();
                    fullDml.append(dml);
                    if(currentDeleteCount>0 && currentDeleteCount%BULK_DATA_OPERATION_CHUNK_SIZE==0){
                        currentDeleteCount=0;
                        rs->endFetchRows();
                        targetDb->executeQueryAndCleanup(QString("begin %1 commit; end;").arg(fullDml));
                        rs->beginFetchRows();
                        fullDml="";
                    }
                }
            }

            emit statusChanged(QString("Comparing data for table %1 (%2 records done - DELETE)").arg(this->tableName).arg(fetchedCount));

            offset=0;

            if(this->stopped){
                break;
            }
        }
    }

    rs->endFetchRows();

    if(offset>0){
        arrSizeParam->setIntValue(offset);
        bulkDeleteHelper.nullifyArrayData(sourceDeleteGeneratorStmt, offset);
        sourceDeleteGeneratorStmt->execute();

        dml=sourceDeleteGeneratorStmt->rsAt(0)->fetchOneAsString(1);
        currentDeleteCount += deleteCountParam->getIntValue();

        if(!dml.isEmpty()){
            fullDml.append(dml);
        }
        if(compareOptions->comparisonMode==DataComparisonOptions::GenerateDml && !dml.isEmpty()){
           emitCompareInfo(tableName, "", 0, 0, currentDeleteCount, dml);
        }else if(!fullDml.isEmpty()){
            emitCompareInfo(tableName, "", 0, 0, currentDeleteCount, "");
            //qDebug() << QString("begin %1 commit; end;").arg(fullDml);
            targetDb->executeQueryAndCleanup(QString("begin %1 commit; end;").arg(fullDml));
        }
    }

    sourceDeleteGeneratorStmt->unlockConnection();

}
