#include "schemaselectordelegate.h"
#include "connectivity/dbconnection.h"
#include "widgets/dbitemlistcombobox.h"
#include <QtGui>

SchemaSelectorDelegate::SchemaSelectorDelegate(const QString &schemaName,
                                               IQueryScheduler *queryScheduler, QObject *parent, bool appendRowIfLast) :
    DbItemListDelegate(schemaName, queryScheduler, "get_schema_list", "user", parent, appendRowIfLast)
{
}
