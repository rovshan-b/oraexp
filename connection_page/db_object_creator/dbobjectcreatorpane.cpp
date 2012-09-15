#include "dbobjectcreatorpane.h"

DbObjectCreatorPane::DbObjectCreatorPane(QWidget *parent) :
    QWidget(parent)
{
}

void DbObjectCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
}
