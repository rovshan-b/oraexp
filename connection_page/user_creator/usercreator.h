#ifndef USERCREATOR_H
#define USERCREATOR_H

#include "connection_page/db_object_creator/dbobjectcreator.h"

class UserCreator : public DbObjectCreator
{
    Q_OBJECT
public:
    explicit UserCreator(const QString &schemaName,
                         const QString &objectName,
                         DbUiManager *uiManager,
                         CreatorMode creatorMode,
                         QWidget *parent = 0);
    
protected:
    virtual DbObjectCreatorPane *createCreatorPane(DbObjectCreator *objectCreator);
    
};

#endif // USERCREATOR_H
