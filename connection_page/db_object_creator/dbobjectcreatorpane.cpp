#include "dbobjectcreatorpane.h"
#include "dbobjectcreator.h"
#include "interfaces/iqueryscheduler.h"

DbObjectCreatorPane::DbObjectCreatorPane(DbObjectCreator *objectCreator, QWidget *parent) :
    QWidget(parent), objectCreator(objectCreator)
{
}

void DbObjectCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
}
