#include "usercreatortab.h"

UserCreatorTab::UserCreatorTab(UserCreatorTabs *userCreator, bool editMode, QWidget *parent) :
    DbObjectCreatorTab(editMode, parent), userCreator(userCreator)
{
}
