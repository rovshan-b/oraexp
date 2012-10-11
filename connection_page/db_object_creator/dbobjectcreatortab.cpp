#include "dbobjectcreatortab.h"

DbObjectCreatorTab::DbObjectCreatorTab(bool editMode, QWidget *parent) :
    QWidget(parent), queryScheduler(0), editMode(editMode)
{
}
