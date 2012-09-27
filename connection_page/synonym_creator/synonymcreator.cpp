#include "synonymcreator.h"
#include "synonymcreatorpane.h"

SynonymCreator::SynonymCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 QWidget *parent) :
    DbObjectCreator(schemaName, objectName, uiManager, DbTreeModel::Synonym, parent)
{
}

DbObjectCreatorPane *SynonymCreator::createCreatorPane(DbObjectCreator *objectCreator)
{
    return new SynonymCreatorPane(objectCreator);
}
