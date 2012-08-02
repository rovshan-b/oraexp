#include "tableconstraintsloader.h"
#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"

TableConstraintsLoader::TableConstraintsLoader(QObject *parent) :
    QObject(parent), fkSubqueryCount(0)
{
    tableInfoLoader=static_cast<TableInfoLoader*>(parent);
    Q_ASSERT(tableInfoLoader);
}

void TableConstraintsLoader::loadInfo(bool checkConstraints)
{
    this->checkConstraints=checkConstraints;
    this->constraints.clear();

    if(checkConstraints){
        tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_check_constraints_for_editing",
                                                           tableInfoLoader->getBasicQueryParams(), this,
                         "get_table_check_constraints_for_editing",
                         "queryCompleted",
                         "recordFetched",
                         "fetchCompleted");
    }else{
        tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_ref_constraints_for_editing",
                                                           tableInfoLoader->getBasicQueryParams(), this,
                         "get_table_ref_constraints_for_editing",
                         "queryCompleted",
                         "recordFetched",
                         "fetchCompleted");
    }
}

void TableConstraintsLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableConstraintsLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    ConstraintInfo consInfo=ConstraintInfo::fromFetchResult(result);
    constraints.append(consInfo);

    if(consInfo.type==OraExp::ForeignKey){
        fkSubqueryCount++;

        tableInfoLoader->getQueryScheduler()->enqueueQuery("get_constraint_column_names",
                         QList<Param*>()
                         << new Param(":owner", consInfo.rOwner)
                         << new Param(":constraint_name", consInfo.rConsName),
                         this,
                         QString("get_constraint_column_names_%1").arg(constraints.size()),
                         "consColNamesQueryCompleted",
                         "consColNamesRecordFetched");
    }
}

void TableConstraintsLoader::fetchCompleted(const QString &/*taskName*/)
{
    if(fkSubqueryCount==0){
        emit infoReady(constraints, this->checkConstraints);
    }
}

void TableConstraintsLoader::consColNamesQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableConstraintsLoader::consColNamesRecordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    int rowId=result.taskName.mid(result.taskName.lastIndexOf('_')+1).toInt();
    Q_ASSERT(rowId>0 && rowId<=constraints.size());

    QString rColumns=result.oneRow.at(0);
    constraints[rowId-1].rColumns=rColumns;

    fkSubqueryCount--;
    Q_ASSERT(fkSubqueryCount>=0);
    if(fkSubqueryCount==0){
        emit infoReady(constraints, this->checkConstraints);
    }
}
