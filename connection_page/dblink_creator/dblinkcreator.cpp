#include "dblinkcreator.h"
#include "dblinkcreatorpane.h"

DbLinkCreator::DbLinkCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::DatabaseLink, parent)
{
}

DbObjectCreatorPane *DbLinkCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new DbLinkCreatorPane(objectCreator);
}
