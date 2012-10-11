#ifndef USERCREATORTAB_H
#define USERCREATORTAB_H

#include "connection_page/db_object_creator/dbobjectcreatortab.h"

class UserCreatorTabs;

class UserCreatorTab : public DbObjectCreatorTab
{
    Q_OBJECT
public:
    explicit UserCreatorTab(UserCreatorTabs *userCreator, bool editMode, QWidget *parent = 0);
    
protected:
    UserCreatorTabs *userCreator;
};

#endif // USERCREATORTAB_H
