#ifndef TABLECREATORTAB_H
#define TABLECREATORTAB_H

#include "connection_page/db_object_creator/dbobjectcreatortab.h"

class TableCreatorTabs;

class TableCreatorTab : public DbObjectCreatorTab
{
    Q_OBJECT
public:
    explicit TableCreatorTab(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent = 0);

protected:
    TableCreatorTabs *tableCreator;
};

#endif // TABLECREATORTAB_H
