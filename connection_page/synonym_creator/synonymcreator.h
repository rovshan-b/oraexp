#ifndef SYNONYMCREATOR_H
#define SYNONYMCREATOR_H

#include "connection_page/db_object_creator/dbobjectcreator.h"

class SynonymCreator : public DbObjectCreator
{
    Q_OBJECT
public:
    explicit SynonymCreator(const QString &schemaName,
                             const QString &objectName,
                             DbUiManager *uiManager,
                             QWidget *parent = 0);
    
protected:
    virtual DbObjectCreatorPane *createCreatorPane(DbObjectCreator *objectCreator);
    
};

#endif // SYNONYMCREATOR_H
