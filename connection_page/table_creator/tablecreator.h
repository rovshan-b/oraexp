#ifndef TABLECREATOR_H
#define TABLECREATOR_H

#include "connection_page/db_object_creator/dbobjectcreator.h"

class DbUiManager;

class TableCreator : public DbObjectCreator
{
    Q_OBJECT
public:
    explicit TableCreator(const QString &schemaName,
                          const QString &objectName,
                          DbUiManager *uiManager,
                          QWidget *parent = 0);

protected:
    virtual DbObjectCreatorPane *createCreatorPane(const QString &schemaName, const QString &objectName);
};

#endif // TABLECREATOR_H
