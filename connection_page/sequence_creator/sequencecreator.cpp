#include "sequencecreator.h"
#include "sequencecreatorpane.h"

SequenceCreator::SequenceCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 CreatorMode creatorMode, QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Sequence, creatorMode, parent)
{
}

DbObjectCreatorPane *SequenceCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new SequenceCreatorPane(objectCreator);
}
