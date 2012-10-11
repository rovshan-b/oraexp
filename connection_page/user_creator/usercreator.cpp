#include "usercreator.h"
#include "usercreatorpane.h"
#include <QtGui>

UserCreator::UserCreator(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Schema, parent)
{
}

DbObjectCreatorPane *UserCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new UserCreatorPane(objectCreator);
}
