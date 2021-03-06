#include "datatable.h"
#include "models/resultsettablemodel.h"
#include "models/scrollableresultsettablemodel.h"
#include "models/editableresultsettablemodel.h"
#include "util/queryqueuemanager.h"
#include "util/queryexectask.h"
#include "util/dbutil.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "util/modelutil.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include "context_menu/contextmenuutil.h"
#include "beans/dynamicaction.h"
#include "connection_page/data_exporter/dataexporterfactory.h"
#include "connection_page/data_exporter/dataexporterobject.h"
#include "defines.h"
#include "errors.h"
#include <QtGui>

DataTable::DataTable(QWidget *parent) :
    QTableView(parent), queryScheduler(0), humanizeColumnNames(false), quietMode(true),
    schemaNameCol(-1), objectNameCol(-1), parentObjectNameCol(-1), objectTypeCol(-1), uiManager(0),
    maxColumnWidth(300), editable(false)
{
    verticalHeader()->setDefaultSectionSize(fontMetrics().height()+10);
    horizontalHeader()->setDefaultSectionSize(150);
    setSelectionMode(QAbstractItemView::ContiguousSelection);

    //connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(indexPressed(QModelIndex)));
}

void DataTable::setResultset(IQueryScheduler *queryScheduler,
                             Resultset *rs,
                             QHash<int, StatementDesc*> dynamicQueries)
{
    this->queryScheduler=queryScheduler;

    deleteCurrentModel();

    if(rs!=0){
        ResultsetTableModel *newModel=0;

        if(rs->isScrollable()){
            newModel = new ScrollableResultsetTableModel(queryScheduler, rs, this, dynamicQueries, iconColumns, humanizeColumnNames);
        }else{
            if(editable){
                newModel = new EditableResultsetTableModel(queryScheduler, rs, this, dynamicQueries, iconColumns, humanizeColumnNames);
            }else{
                newModel = new ResultsetTableModel(queryScheduler, rs, this, dynamicQueries, iconColumns, humanizeColumnNames);
            }
        }

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
        if(result.statement->rsCount() == 0){
            clear();
            delete result.statement;
        }else{
            setResultset(queryScheduler, result.statement->rsAt(0), this->dynamicQueries);
        }
    }

    emit asyncQueryCompleted(result);
}

void DataTable::showContextMenu(const QPoint &pos, QModelIndex index) //pos is in global coordinates
{
    if(!model() || !index.isValid()){
        return;
    }

    QList<QAction*> actions;

    //first add copy actions.
    actions.append(createCopyMenu(false));
    actions.append(createCopyMenu(true));

    actions.append(WidgetHelper::createSeparatorAction());

    int row=index.row();
    actions.append(getActionsForObject(row));

    if(actions.size()==0){
        return;
    }

    QMenu *menu = new QMenu();
    menu->addActions(actions);
    menu->setDefaultAction(WidgetHelper::findDefaultAction(actions));
    connect(menu, SIGNAL(triggered(QAction*)), this, SIGNAL(contextMenuTriggered(QAction*)));
    menu->exec(pos);

    WidgetHelper::deleteMenu(menu);
}

QAction *DataTable::createCopyMenu(bool withHeader) const
{
    QAction *action = new QAction(IconUtil::getIcon("editcopy"), withHeader ? tr("Copy with header") : tr("Copy"), 0);
    QMenu *menu = new QMenu();
    QStringList formats;
    formats << tr("TSV") << tr("CSV") << tr("HTML") << tr("XML");
    foreach(const QString &format, formats){
        menu->addAction(format, this, SLOT(copyAs()))->setData(withHeader);
    }

    action->setMenu(menu);

    return action;
}

void DataTable::invokeDefaultActionForObject(int row)
{
    QList<QAction*> actions = getActionsForObject(row);

    if(actions.size()==0){
        return;
    }

    QAction *action = WidgetHelper::findDefaultAction(actions);
    QAction *actionToTrigger = action ? action : actions.at(0);
    actionToTrigger->trigger();

    WidgetHelper::deleteActions(actions);
}

void DataTable::copyAs()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    Q_ASSERT(action);

    DataExporterBase *exporter = DataExporterFactory::createExporter(action->text());
    exporter->includeColumnHeaders = action->data().toBool();

    copyToClipboard(exporter);
}

void DataTable::setEditable(bool editable)
{
    this->editable = editable;
}

void DataTable::closeEditors(bool commit)
{
    if(!model()){
        return;
    }

    QModelIndex index = currentIndex();

    if(!index.isValid()){
        return;
    }

    if(commit){
        QWidget* editor = indexWidget(index);
        commitData(editor);
    }
    closePersistentEditor(index);
}

bool DataTable::objectListModeEnabled() const
{
    return objectNameCol==-1;
}


QList<QAction *> DataTable::getActionsForObject(int row)
{
    if(objectListModeEnabled()){
        return QList<QAction*>();
    }

    if(!model()){
        return QList<QAction*>();
    }

    QString schemaName = schemaNameCol!=-1 ? model()->index(row, schemaNameCol).data().toString() : objectListSchemaName;
    QString objectName = model()->index(row, objectNameCol).data().toString();
    QString parentObjectName = parentObjectNameCol!=-1 ? model()->index(row, parentObjectNameCol).data().toString() : objectListParentObjectName;
    QString objectType = objectTypeCol!=-1 ? model()->index(row, objectTypeCol).data().toString() : objectListObjectType;

    this->selectedObjectType = objectType;

    QList<QAction*> actions=ContextMenuUtil::getActionsForObject(schemaName, objectName, parentObjectName,
                                         DbUtil::getDbObjectNodeTypeByTypeName(objectType),
                                         this->uiManager, this);

    return actions;
}

void DataTable::copyToClipboard(DataExporterBase *exporter)
{
    if(!model()){
        delete exporter;
        return;
    }

    int startRow, startColumn, endRow, endColumn;
    getSelectedRange(&startRow, &startColumn, &endRow, &endColumn);

    if(startRow==-1 || startColumn==-1 || endRow==-1 || endColumn==-1){
        delete exporter;
        return;
    }

    if(startRow==endRow && startColumn==endColumn){ //disable quoting if single cell is selected
        exporter->stringQuoting = "";
        exporter->numberQuoting = "";
        exporter->quoteColumnHeaders = false;
    }

    exporter->startRow = startRow;
    exporter->startColumn = startColumn;
    exporter->endRow = endRow;
    exporter->endColumn = endColumn;

    for(int i=0; i<model()->columnCount(); ++i){
        exporter->columnTitles.append(model()->headerData(i, Qt::Horizontal).toString());
    }

    DataExporterObject expObj(exporter, ModelUtil::getModelData(model()), 0, false);
    expObj.exportData();

    if(exporter->stringBuffer.endsWith("\r\n")){
        exporter->stringBuffer.chop(2);
    }else if(exporter->stringBuffer.endsWith("\n")){
        exporter->stringBuffer.chop(1);
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(exporter->getMimeType(), exporter->stringBuffer.toUtf8());
    mimeData->setText(exporter->stringBuffer);
    QApplication::clipboard()->setMimeData(mimeData);
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

void DataTable::setActionProperties(DynamicAction *action)
{
    action->properties["DB_OBJECT_TYPE"] = selectedObjectType;
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
    setResultset(this->queryScheduler, 0);
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

    //setContextMenuPolicy(Qt::CustomContextMenu);
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
        DataExporterBase *exporter = DataExporterFactory::createExporter(DataExporterBase::CSV);
        exporter->columnDelimiter = "\t";
        copyToClipboard(exporter);
    }else{
        QTableView::keyPressEvent(event);
    }
}

/*
void DataTable::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);

    if(event->button()==Qt::RightButton){
        showContextMenu(mapToGlobal(event->pos()), QModelIndex());
    }
}*/

void DataTable::indexPressed(const QModelIndex &index)
{
    if(!index.isValid()){
        return;
    }

    if(qApp->mouseButtons()==Qt::RightButton){
        showContextMenu(QCursor::pos(), index);
    }
}


void DataTable::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == 16777301){
        QModelIndex ix = currentIndex();
        if(!ix.isValid()){
            return;
        }
        scrollTo(ix);
        QRect rect = visualRect(ix);
        showContextMenu(viewport()->mapToGlobal(rect.center()), ix);
        return;
    }

    QTableView::keyReleaseEvent(event);
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
