#include "usercreator.h"
#include "usercreatorpane.h"
#include <QtGui>

UserCreator::UserCreator(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, CreatorMode creatorMode, QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Schema, creatorMode, parent)
{
}

DbObjectCreatorPane *UserCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new UserCreatorPane(objectCreator);
}
