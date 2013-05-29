#include "tablecreator.h"
#include "tablecreatorpane.h"
#include <QtGui>

TableCreator::TableCreator(const QString &schemaName,
                           const QString &objectName,
                           DbUiManager *uiManager,
                           bool editMode, QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Table, parent),
    editMode(editMode)
{

}

void TableCreator::highlightChildObject()
{
    this->creatorPane->highlightChildObject();
}

DbObjectCreatorPane *TableCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new TableCreatorPane(objectCreator, editMode);
}
