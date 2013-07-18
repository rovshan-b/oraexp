#include "datatable.h"
#include "models/resultsettablemodel.h"
#include "models/scrollableresultsettablemodel.h"
#include "util/queryqueuemanager.h"
#include "util/queryexectask.h"
#include "util/dbutil.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include "context_menu/contextmenuutil.h"
#include "defines.h"
#include "errors.h"
#include <QtGui>

DataTable::DataTable(QWidget *parent) :
    QTableView(parent), queryScheduler(0), humanizeColumnNames(false), quietMode(true),
    schemaNameCol(-1), objectNameCol(-1), parentObjectNameCol(-1), objectTypeCol(-1),
    maxColumnWidth(300)
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

    deleteCurrentModel();

    if(rs!=0){
        ResultsetTableModel *newModel=rs->isScrollable() ?
                    new ScrollableResultsetTableModel(queryScheduler, rs, this, dynamicQueries, iconColumns, humanizeColumnNames)
                  :
                    new ResultsetTableModel(queryScheduler, rs, this, dynamicQueries, iconColumns, humanizeColumnNames);
        connect(newModel, SIGNAL(firstFetchCompleted()), this, SLOT(handleFirstFetchCompleted()));

        //newModel->setFetchSize(getVisibleRecordCount()+10);
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

void DataTable::handleFirstFetchCompleted()
{
    resizeColumnsToFitContents();

    /*
    ResultsetTableModel *currModel = qobject_cast<ResultsetTableModel*>(this->model());
    if(currModel){
        currModel->setFetchSize(DB_PREFETCH_SIZE);
    }*/

    emit firstFetchCompleted();
}


void DataTable::resizeColumnsToFitContents()
{
    setUpdatesEnabled(false);
    for(int i=0; i<horizontalHeader()->count(); ++i){
        int sizeHint = qMax(sizeHintForColumn(i), horizontalHeader()->sectionSizeHint(i));
        setColumnWidth(i, qMin(sizeHint+10, this->maxColumnWidth));
    }
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
        displayError(tr("Error retrieving data"), result.exception);

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
    QString parentObjectName = parentObjectNameCol!=-1 ? model()->index(row, parentObjectNameCol).data().toString() : objectListParentObjectName;
    QString objectType = objectTypeCol!=-1 ? model()->index(row, objectTypeCol).data().toString() : objectListObjectType;

    QList<QAction*> actions=ContextMenuUtil::getActionsForObject(schemaName, objectName, parentObjectName,
                                         DbUtil::getDbObjectNodeTypeByTypeName(objectType),
                                         this->uiManager);

    if(actions.size()==0){
        return;
    }

    QMenu *menu = new QMenu();
    menu->exec(actions, viewport()->mapToGlobal(pos));

    WidgetHelper::deleteMenu(menu);

}

void DataTable::copyToClipboard()
{
    int startRow, startColumn, endRow, endColumn;
    getSelectedRange(&startRow, &startColumn, &endRow, &endColumn);

    if(startRow==-1 || startColumn==-1 || endRow==-1 || endColumn==-1){
        return;
    }

    bool isMultiColumn = startColumn!=endColumn;
    bool isMultiRow = startRow!=endRow;

    QString selectedText;

    for(int i=startRow; i<=endRow; ++i){
        for(int k=startColumn; k<=endColumn; ++k){
            selectedText.append(this->model()->index(i, k).data().toString());

            if(isMultiColumn && k<endColumn){
                selectedText.append("\t");
            }
        }

        if(isMultiRow && i<endRow){
            selectedText.append("\n");
        }
    }

    QApplication::clipboard()->setText(selectedText);
}

void DataTable::deleteCurrentModel()
{
    WidgetHelper::deleteViewModel(this);
}

void DataTable::displayMessage(const QString &prefix, const OciException &ex)
{
    deleteCurrentModel();

    QStandardItemModel *errModel=new QStandardItemModel(this);
    errModel->setHorizontalHeaderLabels(QStringList() << tr("Error"));

    QStandardItem *errItem=new QStandardItem(QString("%1 - %2").arg(prefix, ex.getErrorMessage()));
    errItem->setIcon(IconUtil::getIcon("error"));
    errModel->appendRow(errItem);

    setModel(errModel);
    resizeColumnToContents(0);
    resizeRowToContents(0);
}

void DataTable::setMaxColumnWidth(int maxColumnWidth)
{
    this->maxColumnWidth = maxColumnWidth;
}

void DataTable::displayError(const QString &prefix, const OciException &ex)
{
    if(quietMode){
        displayMessage(prefix, ex);
    }else{
        QMessageBox::critical(this->window(), prefix, ex.getErrorMessage());
    }
}

int DataTable::getVisibleRecordCount() const
{
    int defaultRowHeight = verticalHeader()->defaultSectionSize();
    int visibleRowCount = qCeil(this->height()/(qreal)defaultRowHeight);
    return visibleRowCount;
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

void DataTable::setObjectListMode(int schemaNameCol, int objectNameCol,
                                  int parentObjectNameCol, int objectTypeCol,
                                  const QString &objectListSchemaName,
                                  const QString &objectListParentObjectName,
                                  const QString &objectListObjectType)
{
    this->schemaNameCol=schemaNameCol;
    this->objectNameCol=objectNameCol;
    this->parentObjectNameCol=parentObjectNameCol;
    this->objectTypeCol=objectTypeCol;
    this->objectListSchemaName=objectListSchemaName;
    this->objectListParentObjectName=objectListParentObjectName;
    this->objectListObjectType=objectListObjectType;

    setContextMenuPolicy(Qt::CustomContextMenu);
}

void DataTable::getSelectedRange(int *startRow, int *startColumn, int *endRow, int *endColumn)
{
    if(this->selectionModel()==0 || !this->selectionModel()->hasSelection()){
        *startRow = -1;
        *endRow = -1;

        if(startColumn!=0){
            *startColumn = -1;
        }
        if(endColumn!=0){
            *endColumn = -1;
        }

        return;
    }

    QModelIndexList indexes = this->selectionModel()->selectedIndexes();

    const QModelIndex &from = indexes.at(0);
    const QModelIndex &to = indexes.at(indexes.size()-1);

    *startRow = from.row();
    *endRow = to.row();

    if(startColumn!=0){
        *startColumn = from.column();
    }
    if(endColumn!=0){
        *endColumn = to.column();
    }
}

void DataTable::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy)){
        copyToClipboard();
    }else{
        QTableView::keyPressEvent(event);
    }
}

/*
void DataTable::resizeEvent(QResizeEvent *event)
{
    ScrollableResultsetTableModel *currModel = qobject_cast<ScrollableResultsetTableModel*>(this->model());
    if(currModel!=0){
        currModel->fetchData();
    }

    QTableView::resizeEvent(event);
}
*/
