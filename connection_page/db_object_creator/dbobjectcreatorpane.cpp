#include "dbobjectcreatorpane.h"

DbObjectCreatorPane::DbObjectCreatorPane(const QString &schemaName, const QString &objectName, QWidget *parent) :
    QWidget(parent), schemaName(schemaName), objectName(objectName)
{
}

void DbObjectCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
}
