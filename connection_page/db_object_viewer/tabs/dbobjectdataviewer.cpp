#include "dbobjectdataviewer.h"
#include "widgets/datatable.h"


DbObjectDataViewer::DbObjectDataViewer(QWidget *parent) :
    DbObjectViewerGenericTab("", parent)
{

}

QList<Param *> DbObjectDataViewer::getQueryParams()
{
    return QList<Param*>();
}

void DbObjectDataViewer::setObjectName(const QString &schemaName, const QString &tableName)
{
    DbObjectViewerWidget::setObjectName(schemaName, tableName);

    query=QString("select * from \"%1\".\"%2\"").arg(schemaName).arg(tableName);
}

void DbObjectDataViewer::createMainWidget(QLayout *layout)
{
    DbObjectViewerGenericTab::createMainWidget(layout);

    dt->setHumanizeColumnNames(false);
}
