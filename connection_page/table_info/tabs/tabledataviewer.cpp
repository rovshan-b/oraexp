#include "tabledataviewer.h"
#include "widgets/datatable.h"


TableDataViewer::TableDataViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{

}

QList<Param *> TableDataViewer::getQueryParams()
{
    return QList<Param*>();
}

void TableDataViewer::setTableName(const QString &schemaName, const QString &tableName)
{
    TableInfoViewerWidget::setTableName(schemaName, tableName);

    query=QString("select * from \"%1\".\"%2\"").arg(schemaName).arg(tableName);
}

void TableDataViewer::createMainWidget(QLayout *layout)
{
    TableInfoGenericTab::createMainWidget(layout);

    dt->setHumanizeColumnNames(false);
}
