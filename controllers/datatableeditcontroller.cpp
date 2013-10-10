#include "datatableeditcontroller.h"
#include "dialogs/descriptiveerrordialog.h"
#include "dialogs/codeviewerdialog.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/statement.h"
#include "models/editableresultsettablemodel.h"
#include "delegates/plaintexteditordelegate.h"
#include "delegates/dataselectordelegate.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include <QtGui>

DataTableEditController::DataTableEditController(DataTable *parent) :
    QObject(parent), dt(parent), queryScheduler(0), addAction(0), editingEnabled(true)
{
    connect(dt, SIGNAL(firstFetchCompleted()), this, SLOT(loadConstraintInfo()));
}

void DataTableEditController::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;
}

void DataTableEditController::setObjectName(const QString &schemaName, const QString &objectName, const QString &dblinkName)
{
    this->schemaName = schemaName;
    this->objectName = objectName;
    this->dblinkName = dblinkName;
}

void DataTableEditController::enableEditActions(bool enable)
{
    addAction->setEnabled(enable);
    deleteAction->setEnabled(enable);
    commitAction->setEnabled(enable);
    rollbackAction->setEnabled(enable);
    showDmlAction->setEnabled(enable);

    this->editingEnabled = enable;
}

QList<QAction *> DataTableEditController::createEditActions()
{
    Q_ASSERT(addAction==0);

    QList<QAction*> actions;

    addAction = new QAction(IconUtil::getIcon("add"), tr("Add record"), this);
    connect(addAction, SIGNAL(triggered()), this, SLOT(addRecord()));
    actions.append(addAction);

    deleteAction = new QAction(IconUtil::getIcon("delete"), tr("Delete record"), this);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteRecord()));
    actions.append(deleteAction);

    actions.append(WidgetHelper::createSeparatorAction(this));

    commitAction = new QAction(IconUtil::getIcon("commit"), tr("Commit changes"), this);
    connect(commitAction, SIGNAL(triggered()), this, SLOT(commit()));
    actions.append(commitAction);

    rollbackAction = new QAction(IconUtil::getIcon("rollback"), tr("Reset changes"), this);
    connect(rollbackAction, SIGNAL(triggered()), this, SLOT(reset()));
    actions.append(rollbackAction);

    showDmlAction = new QAction(IconUtil::getIcon("query"), tr("Show commit DML"), this);
    connect(showDmlAction, SIGNAL(triggered()), this, SLOT(showDml()));
    actions.append(showDmlAction);

    return actions;
}

void DataTableEditController::addRecord()
{
    EditableResultsetTableModel *model = qobject_cast<EditableResultsetTableModel*>(dt->model());

    if(!model){
        return;
    }

    model->insertRows(0, 1);
    int lastInsertedRow = model->insertedRowCount()-1;
    model->setData(model->index(lastInsertedRow, model->columnCount()-1), tr("Auto"));

    QSharedPointer<ResultsetColumnMetadata> metadata = model->getColumnMetadata();
    for(int i=0; i<metadata->getColumnCount()-1; ++i){ //last is rowid
        OraExp::ColumnDataType dataType = metadata->getColumnDataType(i+1);
        if(DbUtil::isDateType(dataType)){
            model->setData(model->index(lastInsertedRow, i), "sysdate");
        }else if(DbUtil::isTimestampType(dataType)){
            model->setData(model->index(lastInsertedRow, i), "systimestamp");
        }
    }

    dt->scrollTo(model->index(lastInsertedRow, 0));
}

void DataTableEditController::deleteRecord()
{
    EditableResultsetTableModel *model = qobject_cast<EditableResultsetTableModel*>(dt->model());

    if(!model){
        return;
    }

    QItemSelectionModel *selModel = dt->selectionModel();

    QModelIndexList rows = selModel->selectedRows();
    if(rows.isEmpty()){
        QMessageBox::information(dt->window(), tr("No selection"),
                                 tr("Please, select one or more rows and try again"));
        return;
    }

    if(QMessageBox::question(dt->window(), tr("Confirm deletion"),
                              tr("Delete selected rows?"),
                              QMessageBox::Ok|QMessageBox::Cancel,
                              QMessageBox::Ok)!=QMessageBox::Ok){
        return;
    }


    for(int i=rows.size()-1; i>=0; --i){
        model->markRowAsDeleted(rows.at(i).row());
    }
}

void DataTableEditController::commit()
{
    EditableResultsetTableModel *model = qobject_cast<EditableResultsetTableModel*>(dt->model());

    if(!model){
        return;
    }

    QString dml = model->generateDmlAsString(this->schemaName, this->objectName, this->dblinkName);

    if(dml.isEmpty()){
        QMessageBox::information(dt->window(), tr("No changes"), tr("Nothing to commit"));
        return;
    }

    if(QMessageBox::question(dt->window(), tr("Confirm commit"),
                              tr("Commit pending changes?"),
                              QMessageBox::Ok|QMessageBox::Cancel,
                              QMessageBox::Ok)!=QMessageBox::Ok){
        return;
    }

    commitDml = QString("BEGIN\n%1\nCOMMIT;\nEND;").arg(dml);

    queryScheduler->enqueueQuery(QString("$%1").arg(commitDml), QList<Param*>(), this, "commit_changes", "commitQueryCompleted");
}

void DataTableEditController::commitQueryCompleted(const QueryResult &result)
{
    delete result.statement;

    if(result.hasError){
        DescriptiveErrorDialog dialog(tr("Error occured"), result.exception, commitDml, dt->window());
        dialog.exec();
    }else{
        emit refreshRequired();
    }

    commitDml.clear();
}

bool DataTableEditController::reset()
{
    EditableResultsetTableModel *model = qobject_cast<EditableResultsetTableModel*>(dt->model());

    if(!model){
        return true;
    }

    if(!model->hasChanges()){
        return true;
    }

    if(QMessageBox::question(dt->window(), tr("Confirm reset"),
                              tr("All pending changes will be lost. Continue?"),
                              QMessageBox::Ok|QMessageBox::Cancel,
                              QMessageBox::Ok)!=QMessageBox::Ok){
        return false;
    }

    model->resetChanges();

    return true;
}

void DataTableEditController::showDml()
{
    EditableResultsetTableModel *model = qobject_cast<EditableResultsetTableModel*>(dt->model());

    if(!model){
        return;
    }

    CodeViewerDialog dialog(dt->window());
    dialog.setWindowTitle(tr("View commit DML"));
    dialog.setCode(model->generateDmlAsString(this->schemaName, this->objectName, this->dblinkName));

    dialog.exec();
}

void DataTableEditController::loadConstraintInfo()
{
    if(!this->editingEnabled){
        return;
    }

    queryScheduler->enqueueQuery("get_table_fk_constraints", QList<Param*>() <<
                                                             new Param("owner", this->schemaName) <<
                                                             new Param("object_name", this->objectName),
                                 this,
                                 "get_table_fk_constraints",
                                 "constraintsQueryCompleted",
                                 "constraintFetched",
                                 "constraintsFetchCompleted",
                                 false,
                                 this->dblinkName);
}

void DataTableEditController::constraintsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(dt->window(), tr("Error retrieving constraint list"), result.exception.getErrorMessage());

        setColumnDelegates();
    }
}

void DataTableEditController::constraintFetched(const FetchResult &fetchResult)
{
    EditableResultsetTableModel *model = qobject_cast<EditableResultsetTableModel*>(dt->model());

    if(!model){
        return;
    }

    if(fetchResult.hasError){
        QMessageBox::critical(dt->window(), tr("Error retrieving constraint list"), fetchResult.exception.getErrorMessage());
        return;
    }

    QStringList columns = fetchResult.colValue("COLUMNS").split(',', QString::SkipEmptyParts);
    foreach(const QString &column, columns){
        int colIx = model->getColumnMetadata()->getColumnIndexByName(column); //1 based
        DataSelectorDelegate *delegate = new DataSelectorDelegate(this->queryScheduler,
                                                                  fetchResult.colValue("R_OWNER"),
                                                                  fetchResult.colValue("R_TABLE_NAME"),
                                                                  this->dblinkName,
                                                                  this);
        dt->setItemDelegateForColumn(colIx - 1, delegate);
    }
}

void DataTableEditController::constraintsFetchCompleted(const QString &)
{
    setColumnDelegates();
}

void DataTableEditController::clearColumnDelegates()
{
    for(int i=0; i<dt->horizontalHeader()->count(); ++i){
        QAbstractItemDelegate *delegate = dt->itemDelegateForColumn(i);
        if(delegate!=0){
            dt->setItemDelegateForColumn(i, 0);
            delegate->deleteLater();
        }
    }
}

void DataTableEditController::setColumnDelegates()
{
    EditableResultsetTableModel *model = qobject_cast<EditableResultsetTableModel*>(dt->model());

    if(!model){
        return;
    }

    ResultsetColumnMetadata *metadata = model->getColumnMetadata().data();
    foreach(unsigned int textColIx, metadata->textColIndexes){

        if(dt->itemDelegateForColumn(textColIx-1)!=0){
            continue;
        }

        PlainTextEditorDelegate *plainTextDelegate = new PlainTextEditorDelegate(tr("Edit field value"), this);
        plainTextDelegate->setAutoAppendRows(false);
        dt->setItemDelegateForColumn(textColIx-1, plainTextDelegate);
    }

}
