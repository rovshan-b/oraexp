#include "dbobjectdataviewer.h"
#include "widgets/datatable.h"
#include "models/editableresultsettablemodel.h"
#include "dialogs/codeviewerdialog.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "beans/resultsetcolumnmetadata.h"
#include "delegates/plaintexteditordelegate.h"
#include <QtGui>

DbObjectDataViewer::DbObjectDataViewer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab("", uiManager, parent)
{

}

QList<Param *> DbObjectDataViewer::getQueryParams()
{
    return QList<Param*>();
}

void DbObjectDataViewer::setObjectName(const QString &schemaName,
                                       const QString &objectName,
                                       DbTreeModel::DbTreeNodeType itemType)
{
    DbObjectViewerGenericTab::setObjectName(schemaName, objectName, itemType);

    query=QString("select t.*, t.rowid from \"%1\".\"%2\" t").arg(schemaName).arg(objectName);
}

void DbObjectDataViewer::createMainWidget(QLayout *layout)
{
    DbObjectViewerGenericTab::createMainWidget(layout);

    dt->setHumanizeColumnNames(false);
    dt->setEditable();
    dt->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(dt, SIGNAL(firstFetchCompleted()), this, SLOT(setColumnDelegates()));
}

QList<QAction *> DbObjectDataViewer::getSpecificToolbarButtons()
{
    QList<QAction*> actions;

    actions.append(WidgetHelper::createSeparatorAction(this));

    QAction *addAction = new QAction(IconUtil::getIcon("add"), tr("Add record"), this);
    connect(addAction, SIGNAL(triggered()), this, SLOT(addRecord()));
    actions.append(addAction);

    QAction *deleteAction = new QAction(IconUtil::getIcon("delete"), tr("Delete record"), this);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteRecord()));
    actions.append(deleteAction);

    actions.append(WidgetHelper::createSeparatorAction(this));

    QAction *commitAction = new QAction(IconUtil::getIcon("commit"), tr("Commit changes"), this);
    connect(commitAction, SIGNAL(triggered()), this, SLOT(commit()));
    actions.append(commitAction);

    QAction *rollbackAction = new QAction(IconUtil::getIcon("rollback"), tr("Reset changes"), this);
    connect(rollbackAction, SIGNAL(triggered()), this, SLOT(reset()));
    actions.append(rollbackAction);

    QAction *showDmlAction = new QAction(IconUtil::getIcon("query"), tr("Show commit DML"), this);
    connect(showDmlAction, SIGNAL(triggered()), this, SLOT(showDml()));
    actions.append(showDmlAction);

    return actions;
}

void DbObjectDataViewer::addRecord()
{
    EditableResultsetTableModel *model = static_cast<EditableResultsetTableModel*>(dt->model());
    model->insertRows(0, 1);
}

void DbObjectDataViewer::deleteRecord()
{
    QItemSelectionModel *selModel = dt->selectionModel();

    QModelIndexList rows = selModel->selectedRows();
    if(rows.isEmpty()){
        QMessageBox::information(this->window(), tr("No selection"),
                                 tr("Please, select one or more rows and try again"));
        return;
    }

    if(QMessageBox::question(this->window(), tr("Confirm deletion"),
                              tr("Delete selected rows?"),
                              QMessageBox::Ok|QMessageBox::Cancel,
                              QMessageBox::Ok)!=QMessageBox::Ok){
        return;
    }

    EditableResultsetTableModel *model = static_cast<EditableResultsetTableModel*>(dt->model());

    for(int i=rows.size()-1; i>=0; --i){
        model->markRowAsDeleted(rows.at(i).row());
    }
}

void DbObjectDataViewer::commit()
{
}

void DbObjectDataViewer::reset()
{
    if(QMessageBox::question(this->window(), tr("Confirm reset"),
                              tr("Reset all pending changes?"),
                              QMessageBox::Ok|QMessageBox::Cancel,
                              QMessageBox::Ok)!=QMessageBox::Ok){
        return;
    }

    EditableResultsetTableModel *model = static_cast<EditableResultsetTableModel*>(dt->model());
    model->resetChanges();
}

void DbObjectDataViewer::showDml()
{
    EditableResultsetTableModel *model = static_cast<EditableResultsetTableModel*>(dt->model());

    CodeViewerDialog dialog(this->window());
    dialog.setWindowTitle(tr("View commit DML"));
    dialog.setCode(model->generateDmlAsString(this->schemaName, this->objectName));

    dialog.exec();
}

void DbObjectDataViewer::setColumnDelegates()
{
    EditableResultsetTableModel *model = static_cast<EditableResultsetTableModel*>(dt->model());
    ResultsetColumnMetadata *metadata = model->getColumnMetadata().data();
    foreach(unsigned int textColIx, metadata->textColIndexes){
        PlainTextEditorDelegate *plainTextDelegate = new PlainTextEditorDelegate(tr("Edit field value"), this);
        plainTextDelegate->setAutoAppendRows(false);
        dt->setItemDelegateForColumn(textColIx-1, plainTextDelegate);
    }

}
