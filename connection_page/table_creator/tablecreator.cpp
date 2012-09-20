#include "tablecreator.h"
#include "tablecreatorpane.h"
#include <QDebug>
#include <QtGui>

TableCreator::TableCreator(const QString &schemaName,
                           const QString &objectName,
                           DbUiManager *uiManager,
                           QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Table, parent)
{

}

DbObjectCreatorPane *TableCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new TableCreatorPane(objectCreator);
}
