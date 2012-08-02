#include "datatable.h"
#include "models/resultsettablemodel.h"
#include "util/queryqueuemanager.h"
#include "util/queryexectask.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

DataTable::DataTable(QWidget *parent) :
    QTableView(parent), queryScheduler(0), humanizeColumnNames(false)
{
    verticalHeader()->setDefaultSectionSize(fontMetrics().height()+10);
    horizontalHeader()->setDefaultSectionSize(150);
}

void DataTable::setResultset(IQueryScheduler *queryScheduler,
                             Resultset *rs,
                             QHash<int, StatementDesc*> dynamicQueries)
{
    this->queryScheduler=queryScheduler;

    QAbstractItemModel *currentModel=model();
    QItemSelectionModel *currentSelectionModel=selectionModel();

    if(currentModel!=0){
        delete currentModel;
    }

    if(currentSelectionModel!=0){
        delete currentSelectionModel;
    }

    if(rs!=0){
        ResultsetTableModel *newModel=new ResultsetTableModel(queryScheduler, rs, this, dynamicQueries, iconColumns, humanizeColumnNames);
        connect(newModel, SIGNAL(firstFetchCompleted()), this, SLOT(resizeColumnsToFitContents()));
        connect(newModel, SIGNAL(firstFetchCompleted()), this, SIGNAL(firstFetchCompleted()));

        setModel(newModel);

        //resizeColumnsToContents();

        if(newModel->canFetchMore(QModelIndex())){
            newModel->fetchMore(QModelIndex());
        }

        connect(this->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)));
    }else{
        setModel(0);
    }
}

void DataTable::resizeColumnsToFitContents()
{
    setUpdatesEnabled(false);
    resizeColumnsToContents();
    setUpdatesEnabled(true);
}

void DataTable::displayQueryResults(IQueryScheduler *queryScheduler, const QString &query, QList<Param*> params,
                                    QHash<int, StatementDesc*> dynamicQueries)
{
    this->queryScheduler=queryScheduler;

    QueryExecTask task;
    task.query=query;
    task.params=params;
    task.taskName="data_table_task";
    task.requester=this;
    task.queryCompletedSlotName="queryCompleted";

    this->dynamicQueries=dynamicQueries;

    queryScheduler->enqueueQuery(task);
}

void DataTable::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving data"),
                              result.exception.getErrorMessage());

        emit asyncQueryError(result.exception);
    }else{
        setResultset(queryScheduler, result.statement->rsAt(0), this->dynamicQueries);
    }

    emit asyncQueryCompleted(result);
}

void DataTable::clear()
{
    setResultset(0, 0);
}

void DataTable::setIconColumn(const QString &displayColumnName, const QString &iconColumnName)
{
    iconColumns[displayColumnName]=iconColumnName;
}

void DataTable::resizeColumnAccountingForEditor(int column)
{
    this->setColumnWidth(column,
                         qMin(qMax(this->sizeHintForColumn(column)+20, this->horizontalHeader()->defaultSectionSize()),
                             300));
}

void DataTable::resizeColumnsAccountingForEditor()
{
    if(!this->model()){
        return;
    }

    for(int i=0; i<this->model()->columnCount(); ++i){
        if(isColumnHidden(i)){
            continue;
        }

        resizeColumnAccountingForEditor(i);
    }
}
