#ifndef SEQUENCECREATOR_H
#define SEQUENCECREATOR_H

#include "connection_page/db_object_creator/dbobjectcreator.h"

class SequenceCreator : public DbObjectCreator
{
    Q_OBJECT
public:
    explicit SequenceCreator(const QString &schemaName,
                             const QString &objectName,
                             DbUiManager *uiManager,
                             QWidget *parent = 0);
    
protected:
    virtual DbObjectCreatorPane *createCreatorPane(const QString &schemaName, const QString &objectName);
    
};

#endif // SEQUENCECREATOR_H
