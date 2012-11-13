#include "tabledataviewer.h"
#include "widgets/datatable.h"


TableDataViewer::TableDataViewer(QWidget *parent) :
    DbObjectViewerGenericTab("", parent)
{

}

QList<Param *> TableDataViewer::getQueryParams()
{
    return QList<Param*>();
}

void TableDataViewer::setObjectName(const QString &schemaName, const QString &tableName)
{
    DbObjectViewerWidget::setObjectName(schemaName, tableName);

    query=QString("select * from \"%1\".\"%2\"").arg(schemaName).arg(tableName);
}

void TableDataViewer::createMainWidget(QLayout *layout)
{
    DbObjectViewerGenericTab::createMainWidget(layout);

    dt->setHumanizeColumnNames(false);
}
