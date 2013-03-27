#include "dataoperationthread.h"
#include "util/queryutil.h"
#include "util/dbutil.h"
#include "connectivity/queryresult.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "defines.h"

DataOperationThread::DataOperationThread(const QString &sourceSchema, DbConnection *sourceDb, const QString &targetSchema, DbConnection *targetDb, const QString &tableName, DataOperationOptions *options, const TableInfoForDataComparison &tableOptions, QObject *parent) :
    QThread(parent),
    sourceSchema(sourceSchema),
    sourceDb(sourceDb),
    targetSchema(targetSchema),
    targetDb(targetDb),
    tableName(tableName),
    options(options),
    tableOptions(tableOptions),
    stopped(false)
{
}


void DataOperationThread::emitCompletedSignal()
{
    emit comparisonCompleted();
}

void DataOperationThread::emitCompareInfo(const QString &tableName, const QString &newStatus, int inserts, int updates, int deletes, const QString &dml)
{
    DataCompareInfo info=DataCompareInfo(tableName, newStatus, inserts, updates, deletes);
    info.dml=dml;

    emit compareInfoAvailable(info);
}

void DataOperationThread::prepareBindArrays()
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


        prepareBindArrayForColumn(colName, dataType, length, offset);

        ++offset;
    }
    rs->endFetchRows();
}

QString DataOperationThread::toDynamicSqlValue(const QString &varName, const QString &dataType)
{
    QString result;

    if(DbUtil::isStringType(dataType)){

        result = QString("'''||replace(%1,'''','''||chr(39)||''')||'''").arg(varName);
    }else if(DbUtil::isNumericType(dataType)){
        result = QString("to_number('''||%1||''')").arg(varName);
    }else if(DbUtil::isDateType(dataType)){
        result = QString("to_date('''||to_char(%1,'%2')||''',''%2'')").arg(varName, DB_DATE_FORMAT);
    }else if(DbUtil::isIntervalType(dataType)){
        OraExp::ColumnSubType intervalSubType=DbUtil::getIntervalSubType(dataType);
#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
        if(intervalSubType==OraExp::CSTIntervalDs){
            result = QString("to_dsinterval('''||%1||''')").arg(varName);
        }else{
            result = QString("to_yminterval('''||%1||''')").arg(varName);
        }
#else
        if(intervalSubType==OraExp::CSTIntervalDs){
            result = QString("to_dsinterval('''||to_char(%1)||''')").arg(varName);
        }else{
            result = QString("to_yminterval('''||to_char(%1)||''')").arg(varName);
        }
#endif
    }else if(DbUtil::isTimestampType(dataType)){
        OraExp::ColumnSubType timestampSubType=DbUtil::getTimestampSubType(dataType);
#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
        if(timestampSubType==OraExp::CSTTimestampTz){
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

QString DataOperationThread::getColumnsForSelect(const QStringList &columnList)
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
