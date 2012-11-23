#include "datacomparerthread.h"
#include "util/queryutil.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "util/strutil.h"
#include <QDebug>
#include "util/dbutil.h"
#include "beans/datacomparisonoptions.h"
#include "defines.h"

const int DATA_COMPARER_CHUNK_SIZE = 50;

DataComparerThread::DataComparerThread(const QString &sourceSchema,
                                       DbConnection *sourceDb,
                                       const QString &targetSchema,
                                       DbConnection *targetDb,
                                       const QString &tableName,
                                       DataComparisonOptions *options,
                                       const TableInfoForDataComparison &tableOptions,
                                       QObject *parent) :
    QThread(parent),
    sourceSchema(sourceSchema),
    sourceDb(sourceDb),
    targetSchema(targetSchema),
    targetDb(targetDb),
    tableName(tableName),
    options(options),
    tableOptions(tableOptions),
    targetStmt(0),
    sourceDeleteGeneratorStmt(0)
{
    compareScript=QueryUtil::getQuery("compare_data", targetDb);
    reverseCompareScript=QueryUtil::getQuery("compare_data_reverse", sourceDb);

    bulkHelper.setBulkSize(DATA_COMPARER_CHUNK_SIZE);
    bulkDeleteHelper.setBulkSize(DATA_COMPARER_CHUNK_SIZE);
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
        QStringList uqColumns = splitTrimmed(uqCols);
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
        prepareBindArrays(uqColumns);

        currentTaskName="create_comparison_script";
        createComparisonScript(uqColumns);

        if(options->deletes){
            currentTaskName="reverse_compare_data";
            doReverseComparison(uqColumns);
        }

        if(options->inserts || options->updates){
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

void DataComparerThread::prepareBindArrays(const QStringList &uqColumns)
{
    QString query=QueryUtil::getQuery("get_table_columns_for_buffer_allocation");
    QueryResult res=sourceDb->executeQuery(query, QList<Param*>() <<
                                  new Param(":owner", sourceSchema) <<
                                  new Param(":object_name", tableName));

    QScopedPointer<Statement> stmt(res.statement);

    Resultset *rs=stmt->rsAt(0);

    rs->beginFetchRows();

    QString colName;
    int offset=0;
    while(rs->moveNext()){

        colName=rs->getString(1);
        QString dataType=rs->getString(2);
        int length=rs->getString(4).toInt();

#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
        allColumns[colName]=dataType;

        if(DbUtil::isIntervalType(dataType) || DbUtil::isTimestampType(dataType)){
            dataType="VARCHAR2";
            length=50;
        }
#endif

        if(!tableOptions.columnsToCompare.contains(colName)){ //column is not comparable
            continue;
        }


        if(options->inserts || options->deletes){
            bulkHelper.createBufferForDataType(targetStmt, dataType, length);
        }

        if(options->deletes && uqColumns.contains(colName)){
            bulkDeleteHelper.createBufferForDataType(sourceDeleteGeneratorStmt, dataType, length);
            uqColumnOffsets.append(offset);
        }

        ++offset;
    }
    rs->endFetchRows();
}

void DataComparerThread::createComparisonScript(const QStringList &uqColumns)
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
            updateScript.append("\n\t\t\t if l_update_sql is not null then l_update_sql := l_update_sql || ','; end if;");
            updateScript.append("\n\t\t\t l_update_sql := l_update_sql || '\"").
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
    QString scriptTargetTableName = (options->comparisonMode==DataComparisonOptions::GenerateDml && !options->includeSchemaName) ?
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
    qDebug() << "reverse compare script:" << reverseCompareScript;
}

QString DataComparerThread::toDynamicSqlValue(const QString &varName, const QString &dataType)
{
    QString result;

    if(DbUtil::isStringType(dataType)){

        result = QString("'''||replace(%1,'''','''||chr(39)||''')||'''").arg(varName);
    }else if(DbUtil::isNumericType(dataType)){
        result = QString("to_number('''||%1||''')").arg(varName);
    }else if(DbUtil::isDateType(dataType)){
        result = QString("to_date('''||to_char(%1,'%2')||''',''%2'')").arg(varName, DB_DATE_FORMAT);
    }else if(DbUtil::isIntervalType(dataType)){
        int intervalType=DbUtil::getIntervalType(dataType);
#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
        if(intervalType==OCI_INTERVAL_DS){
            result = QString("to_dsinterval('''||%1||''')").arg(varName);
        }else{
            result = QString("to_yminterval('''||%1||''')").arg(varName);
        }
#else
        if(intervalType==OCI_INTERVAL_DS){
            result = QString("to_dsinterval('''||to_char(%1)||''')").arg(varName);
        }else{
            result = QString("to_yminterval('''||to_char(%1)||''')").arg(varName);
        }
#endif
    }else if(DbUtil::isTimestampType(dataType)){
        int timestampType=DbUtil::getTimestampType(dataType);
#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
        if(timestampType==OCI_TIMESTAMP_TZ){
            result = QString("to_timestamp_tz('''||%1||''',''%2'')").arg(varName, DB_TZ_TIMESTAMP_FORMAT);
        }else{
            result = QString("to_timestamp('''||%1||''',''%2'')").arg(varName, DB_TIMESTAMP_FORMAT);
        }
#else
        if(timestampType==OCI_TIMESTAMP_TZ){
            result = QString("to_timestamp_tz('''||to_char(%1,'%2')||''',''%2'')").arg(varName, DB_TZ_TIMESTAMP_FORMAT);
        }else{
            result = QString("to_timestamp('''||to_char(%1,'%2')||''',''%2'')").arg(varName, DB_TIMESTAMP_FORMAT);
        }
#endif
    }else{
        qDebug("Data type not supported in DataComparerThread::toDynamicSqlValue");
        Q_ASSERT(false);
    }

    return result;
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
                                              new Param(":generate_inserts", this->options->inserts) <<
                                              new Param(":generate_updates", this->options->updates) <<
                                              new Param(":update_database", (int)this->options->comparisonMode) <<
                                              new Param(":rs_out");
    targetStmt->bindParams(params); //second. do not move upper than first
    targetStmt->printBindVars();

    rs->beginFetchRows();

    int fetchedCount=0;

    QString dml;

    while(rs->moveNext()){

        ++fetchedCount;

        for(int colIx=0; colIx<tableOptions.columnsToCompare.size(); ++colIx){
            bulkHelper.setArrayData(targetStmt, rs, colIx+1, offset);
        }

        ++offset;
        if(offset==DATA_COMPARER_CHUNK_SIZE){
            arrSizeParam->setIntValue(offset);
            targetStmt->execute();

            dml=targetStmt->rsAt(0)->fetchOneAsString(1);

            targetStmt->releaseResultsets();

            emitCompareInfo(tableName, "", insertCountParam->getIntValue(), updateCountParam->getIntValue(), 0, dml);

            emit statusChanged(QString("Comparing data for table %1 (%2 records done - INSERT/UPDATE)").arg(this->tableName).arg(fetchedCount));

            offset=0;
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

void DataComparerThread::doReverseComparison(const QStringList &uqColumns)
{
    emitCompareInfo(tableName, tr("Generating DELETE statements"));

    QString selectFromTargetSql=QString("SELECT %1 FROM \"%2\".\"%3\"").
            arg(getColumnsForSelect(uqColumns)).
            arg(targetSchema).arg(tableOptions.targetTableName);

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
    sourceDeleteGeneratorStmt->printBindVars();

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
        if(offset==DATA_COMPARER_CHUNK_SIZE){
            arrSizeParam->setIntValue(offset);
            sourceDeleteGeneratorStmt->execute();

            dml=sourceDeleteGeneratorStmt->rsAt(0)->fetchOneAsString(1);

            sourceDeleteGeneratorStmt->releaseResultsets();

            if(!dml.isEmpty()){
                if(options->comparisonMode==DataComparisonOptions::GenerateDml){
                    emitCompareInfo(tableName, "", 0, 0, deleteCountParam->getIntValue(), dml);
                }else{
                    emitCompareInfo(tableName, "", 0, 0, deleteCountParam->getIntValue(), "");
                    currentDeleteCount+=deleteCountParam->getIntValue();
                    fullDml.append(dml);
                    if(currentDeleteCount>0 && currentDeleteCount%DATA_COMPARER_CHUNK_SIZE==0){
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
        if(options->comparisonMode==DataComparisonOptions::GenerateDml && !dml.isEmpty()){
           emitCompareInfo(tableName, "", 0, 0, currentDeleteCount, dml);
        }else if(!fullDml.isEmpty()){
            emitCompareInfo(tableName, "", 0, 0, currentDeleteCount, "");
            //qDebug() << QString("begin %1 commit; end;").arg(fullDml);
            targetDb->executeQueryAndCleanup(QString("begin %1 commit; end;").arg(fullDml));
        }
    }

    sourceDeleteGeneratorStmt->unlockConnection();

}

QString DataComparerThread::getColumnsForSelect(const QStringList &columnList)
{
#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL

    QString result;
    for(int i=0; i<columnList.size(); ++i){
        const QString &columnName = columnList.at(i);
        const QString &dataType=allColumns.value(columnName);
        Q_ASSERT(!dataType.isEmpty());

        QString convertedName = DbUtil::intervalOrTimestampToChar(columnName, dataType);

        if(convertedName==columnName){
            result.append("\"").append(convertedName).append("\"");
        }else{
            result.append(convertedName);
        }

        if(i!=columnList.size()-1){
            result.append(",");
        }
    }

    return result;
#else
    return joinEnclosed(columnList, ",", "\"");
#endif
}

void DataComparerThread::emitCompletedSignal()
{
    emit comparisonCompleted();
}

void DataComparerThread::emitCompareInfo(const QString &tableName, const QString &newStatus, int inserts, int updates, int deletes, const QString &dml)
{
    DataCompareInfo info=DataCompareInfo(tableName, newStatus, inserts, updates, deletes);
    info.dml=dml;

    emit compareInfoAvailable(info);
}
