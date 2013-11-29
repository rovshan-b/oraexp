#include "dbobjectdataviewer.h"
#include "widgets/datatable.h"
#include "models/editableresultsettablemodel.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include "util/strutil.h"
#include "beans/resultsetcolumnmetadata.h"
#include "dialogs/orderbyoptionsdialog.h"
#include "dialogs/plaintexteditordialog.h"
#include "controllers/datatableeditcontroller.h"
#include "widgets/genericqueryviewertabs.h"
#include "widgets/clickablelabel.h"
#include "connection_page/dbuimanager.h"
#include "errors.h"
#include <QtGui>

DbObjectDataViewer::DbObjectDataViewer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab("", uiManager, parent), editController(0), filterLabel(0), isEditableResultset(true)
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
        //whereClause = QString("WHERE ROWNUM <= 50000");
    }

    rebuildQuery();
}

void DbObjectDataViewer::rebuildQuery()
{
    query = baseQuery;

    appendWhereClause(query);
    displayFilterText();

    if(!orderBy.isEmpty()){
        query.append(QString(" %1").arg(orderBy));
    }

    if(editController && isEditableResultset){
        editController->enableEditActions(true);
    }

    qDebug() << query;
}

void DbObjectDataViewer::appendWhereClause(QString &appendTo)
{
    if(!whereClause.isEmpty()){
        appendTo.append(QString(" %1").arg(ensureStartsWith(whereClause, "WHERE")));
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

    QAction *countRowsAction = new QAction(IconUtil::getIcon("count"), tr("Count rows"), this);
    connect(countRowsAction, SIGNAL(triggered()), this, SLOT(countRows()));
    actions.append(countRowsAction);

    actions.append(WidgetHelper::createSeparatorAction(this));

    actions.append(editController->createEditActions());

    actions.append(WidgetHelper::createSeparatorAction(this));

    QAction *filterAction = new QAction(IconUtil::getIcon("filter"), tr("Filter"), this);
    connect(filterAction, SIGNAL(triggered()), this, SLOT(filter()));
    actions.append(filterAction);

    return actions;
}

QList<QWidget *> DbObjectDataViewer::getSpecificToolbarWidgets()
{
    filterLabel = new ClickableLabel();
    displayFilterText();

    connect(filterLabel, SIGNAL(clicked()), this, SLOT(filter()));

    return QList<QWidget*>() << filterLabel;
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
    dialog.setQueryScheduler(this->queryScheduler);

    if(dialog.exec()){
        orderBy = dialog.getOrderByClause();
        if(!orderBy.isEmpty()){
            orderBy = orderBy.arg(QString("\"%1\"").arg(dt->model()->headerData(colIx, Qt::Horizontal).toString()));
        }
        rebuildQuery();
        refreshInfo();
    }
}

void DbObjectDataViewer::countRows()
{
    QString queryToRun = QString("select 'Row count: '||trim(to_char(count(0), '999G999G999G999G999G999G999')) from \"%1\".\"%2\"").arg(schemaName).arg(objectName);

    appendWhereClause(queryToRun);

    uiManager->showRecordCount(queryToRun, QList<Param*>());
}

void DbObjectDataViewer::asyncQueryError(const OciException &ex)
{
    editController->enableEditActions(false);

    if(ex.getErrorCode() == ERR_CANNOT_SELECT_ROWID1 ||
            ex.getErrorCode() == ERR_CANNOT_SELECT_ROWID2){

        dt->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->isEditableResultset = false;

        baseQuery = QString("select * from \"%1\".\"%2\"").arg(schemaName).arg(objectName);

        rebuildQuery();
        refreshInfo();
    }
}

void DbObjectDataViewer::displayFilterText()
{
    if(!filterLabel){
        return;
    }

    filterLabel->setToolTip(this->whereClause);

    QString filterText(this->whereClause);
    filterText.replace('\n', ' ');
    QFontMetrics metrics(filterLabel->font());
    filterText = metrics.elidedText(filterText, Qt::ElideRight, 250);
    filterLabel->setText(filterText);
}
