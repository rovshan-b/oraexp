#ifndef DBLINKCREATOR_H
#define DBLINKCREATOR_H

#include "connection_page/db_object_creator/dbobjectcreator.h"

class DbLinkCreator : public DbObjectCreator
{
    Q_OBJECT
public:
    explicit DbLinkCreator(const QString &schemaName,
                             const QString &objectName,
                             DbUiManager *uiManager,
                             QWidget *parent = 0);
    
protected:
    virtual DbObjectCreatorPane *createCreatorPane(DbObjectCreator *objectCreator);
    
};

#endif // DBLINKCREATOR_H
