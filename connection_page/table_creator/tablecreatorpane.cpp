#include "tablecreatorpane.h"
#include "tablecreatortabs.h"
#include <QtGui>

TableCreatorPane::TableCreatorPane(const QString &schemaName, const QString &tableName, QWidget *parent) :
    DbObjectCreatorPane(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);

    tableCreatorTabs=new TableCreatorTabs(schemaName, tableName);

    mainLayout->addWidget(tableCreatorTabs);
    setLayout(mainLayout);

    connect(tableCreatorTabs, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(tableCreatorTabs, SIGNAL(objectInfoLoaded()), this, SIGNAL(objectInfoLoaded()));
}

void TableCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorPane::setQueryScheduler(queryScheduler);

    tableCreatorTabs->setQueryScheduler(queryScheduler);
}

QString TableCreatorPane::generateCreateDdl()
{
    return tableCreatorTabs->getTableInfo().generateDdl();
}

QList<QueryListItem> TableCreatorPane::generateAlterDdl()
{
    return tableCreatorTabs->generateAlterDdl();
}

bool TableCreatorPane::beforeAlter()
{
    tableCreatorTabs->removeIncorrectRows();
    return tableCreatorTabs->beforeAlterTable();
}

QString TableCreatorPane::getSchemaName() const
{
    return tableCreatorTabs->getSchemaName();
}

QString TableCreatorPane::getObjectName() const
{
    return tableCreatorTabs->getTableName();
}
