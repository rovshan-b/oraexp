#include "sequencecreator.h"
#include "sequencecreatorpane.h"

SequenceCreator::SequenceCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Sequence, parent)
{
}

DbObjectCreatorPane *SequenceCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new SequenceCreatorPane(objectCreator);
}
