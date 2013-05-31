#include "synonymcreator.h"
#include "synonymcreatorpane.h"

SynonymCreator::SynonymCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 CreatorMode creatorMode, QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Synonym, creatorMode, parent)
{
}

DbObjectCreatorPane *SynonymCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new SynonymCreatorPane(objectCreator);
}
