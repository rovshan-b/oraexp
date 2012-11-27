#include "datatable.h"
#include "models/resultsettablemodel.h"
#include "util/queryqueuemanager.h"
#include "util/queryexectask.h"
#include "util/dbutil.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include "context_menu/contextmenuutil.h"
#include <QtGui>

DataTable::DataTable(QWidget *parent) :
    QTableView(parent), queryScheduler(0), humanizeColumnNames(false),
    schemaNameCol(-1), objectNameCol(-1), objectTypeCol(-1)
{
    verticalHeader()->setDefaultSectionSize(fontMetrics().height()+10);
    horizontalHeader()->setDefaultSectionSize(150);
    setSelectionMode(QAbstractItemView::ContiguousSelection);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
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

void DataTable::showContextMenu(const QPoint &pos)
{
    if(objectNameCol==-1){
        return;
    }

    if(!model()){
        return;
    }

    QModelIndex index=indexAt(pos);
    if(!index.isValid()){
        return;
    }

    int row=index.row();
    QString schemaName = schemaNameCol!=-1 ? model()->index(row, schemaNameCol).data().toString() : objectListSchemaName;
    QString objectName = model()->index(row, objectNameCol).data().toString();
    QString objectType = objectTypeCol!=-1 ? model()->index(row, objectTypeCol).data().toString() : objectListObjectType;

    QList<QAction*> actions=ContextMenuUtil::getActionsForObject(schemaName, objectName,
                                         DbUtil::getDbObjectNodeTypeByTypeName(objectType),
                                         this->uiManager);

    if(actions.size()==0){
        return;
    }

    QMenu menu;
    menu.exec(actions, viewport()->mapToGlobal(pos));

    qDeleteAll(actions);

}

void DataTable::copyToClipboard()
{
    if(!this->selectionModel()->hasSelection()){
        return;
    }

    QModelIndexList indexes = this->selectionModel()->selectedIndexes();
    QString selectedText;

    const QModelIndex &from = indexes.at(0);
    const QModelIndex &to = indexes.at(indexes.size()-1);

    bool isMultiColumn = from.column()!=to.column();
    bool isMultiRow = from.row()!=to.row();

    for(int i=from.row(); i<=to.row(); ++i){
        for(int k=from.column(); k<=to.column(); ++k){
            selectedText.append(this->model()->index(i, k).data().toString());

            if(isMultiColumn && k<to.column()){
                selectedText.append("\t");
            }
        }

        if(isMultiRow && i<to.row()){
            selectedText.append("\n");
        }
    }

    QApplication::clipboard()->setText(selectedText);
}

void DataTable::clear()
{
    setResultset(0, 0);
}

void DataTable::setIconColumn(const QString &displayColumnName, const QString &iconColumnName)
{
    if(displayColumnName.isEmpty() || iconColumnName.isEmpty()){
        return;
    }

    iconColumns[displayColumnName]=iconColumnName;
}

void DataTable::setIconColumns(const QHash<QString, QString> &iconColumns)
{
    this->iconColumns=iconColumns;
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

void DataTable::setUiManager(DbUiManager *uiManager)
{
    this->uiManager=uiManager;
}

void DataTable::setObjectListMode(int schemaNameCol, int objectNameCol, int objectTypeCol,
                                  const QString &objectListSchemaName, const QString &objectListObjectType)
{
    this->schemaNameCol=schemaNameCol;
    this->objectNameCol=objectNameCol;
    this->objectTypeCol=objectTypeCol;
    this->objectListSchemaName=objectListSchemaName;
    this->objectListObjectType=objectListObjectType;

    setContextMenuPolicy(Qt::CustomContextMenu);
}

void DataTable::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy)){
        copyToClipboard();
    }else{
        QTableView::keyPressEvent(event);
    }
}
