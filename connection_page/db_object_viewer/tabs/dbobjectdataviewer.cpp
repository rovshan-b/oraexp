#include "dbobjectdataviewer.h"
#include "widgets/datatable.h"


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

    query=QString("select * from \"%1\".\"%2\"").arg(schemaName).arg(objectName);
}

void DbObjectDataViewer::createMainWidget(QLayout *layout)
{
    DbObjectViewerGenericTab::createMainWidget(layout);

    dt->setHumanizeColumnNames(false);
}
