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
                          bool editMode,
                          QWidget *parent = 0);

    virtual void highlightChildObject();

protected:
    virtual DbObjectCreatorPane *createCreatorPane(DbObjectCreator *objectCreator);

private:
    bool editMode;
};

#endif // TABLECREATOR_H
