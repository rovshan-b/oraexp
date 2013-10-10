#include "dbobjectdataviewer.h"
#include "widgets/datatable.h"
#include "models/editableresultsettablemodel.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include "beans/resultsetcolumnmetadata.h"
#include "dialogs/orderbyoptionsdialog.h"
#include "dialogs/plaintexteditordialog.h"
#include "controllers/datatableeditcontroller.h"
#include "errors.h"
#include <QtGui>

DbObjectDataViewer::DbObjectDataViewer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab("", uiManager, parent), editController(0)
{

}

void DbObjectDataViewer::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectViewerGenericTab::setQueryScheduler(queryScheduler);

    editController->setObjectName(this->schemaName, this->objectName, "");
    editController->setQueryScheduler(queryScheduler);
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

    if(baseQuery.isEmpty()){
        baseQuery = QString("select t.*, t.rowid from \"%1\".\"%2\" t").arg(schemaName).arg(objectName);
        whereClause = QString("WHERE ROWNUM <= 50000");
    }

    rebuildQuery();
}

void DbObjectDataViewer::rebuildQuery()
{
    query = baseQuery;

    if(!whereClause.isEmpty()){
        query.append(QString(" %1").arg(whereClause));
    }

    if(!orderBy.isEmpty()){
        query.append(QString(" %1").arg(orderBy));
    }

    if(editController){
        editController->enableEditActions(true);
    }
}

void DbObjectDataViewer::createMainWidget(QLayout *layout)
{
    DbObjectViewerGenericTab::createMainWidget(layout);

    Q_ASSERT(editController==0);

    editController = new DataTableEditController(dt) ;

    dt->setHumanizeColumnNames(false);
    dt->setEditable();
    dt->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(dt->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sort(int)));
    connect(dt, SIGNAL(asyncQueryError(OciException)), this, SLOT(asyncQueryError(OciException)));
    connect(editController, SIGNAL(refreshRequired()), this, SLOT(refreshInfo()));
}

QList<QAction *> DbObjectDataViewer::getSpecificToolbarButtons()
{
    QList<QAction*> actions;

    actions.append(WidgetHelper::createSeparatorAction(this));

    actions.append(editController->createEditActions());

    actions.append(WidgetHelper::createSeparatorAction(this));

    QAction *filterAction = new QAction(IconUtil::getIcon("filter"), tr("Filter"), this);
    connect(filterAction, SIGNAL(triggered()), this, SLOT(filter()));
    actions.append(filterAction);

    return actions;
}

void DbObjectDataViewer::filter()
{
    if(!dt->model()){
        return;
    }

    if(!editController->reset()){
        return;
    }

    PlainTextEditorDialog dialog(this, true);
    dialog.setWindowTitle(tr("Edit filter"));
    dialog.setEditorText(whereClause);
    if(dialog.exec()){
        whereClause = dialog.getEditorText();

        rebuildQuery();
        refreshInfo();
    }
}

void DbObjectDataViewer::sort(int colIx)
{
    if(!dt->model()){
        return;
    }

    if(!editController->reset()){
        return;
    }

    OrderByOptionsDialog dialog(this->window());

    if(dialog.exec()){
        orderBy = dialog.getOrderByClause();
        if(!orderBy.isEmpty()){
            orderBy = orderBy.arg(colIx+1);
        }
        rebuildQuery();
        refreshInfo();
    }
}

void DbObjectDataViewer::asyncQueryError(const OciException &ex)
{
    editController->enableEditActions(false);

    if(ex.getErrorCode() == ERR_CANNOT_SELECT_ROWID1 ||
            ex.getErrorCode() == ERR_CANNOT_SELECT_ROWID2){

        dt->setEditTriggers(QAbstractItemView::NoEditTriggers);

        baseQuery = QString("select * from \"%1\".\"%2\"").arg(schemaName).arg(objectName);

        rebuildQuery();
        refreshInfo();
    }
}
