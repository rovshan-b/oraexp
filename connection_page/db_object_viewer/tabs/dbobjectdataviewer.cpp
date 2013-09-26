#include "dbobjectdataviewer.h"
#include "widgets/datatable.h"
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
}
