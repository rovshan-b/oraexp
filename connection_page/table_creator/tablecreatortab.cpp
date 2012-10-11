#include "tablecreatortab.h"
#include "connectivity/dbconnection.h"

TableCreatorTab::TableCreatorTab(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent) :
    DbObjectCreatorTab(editMode, parent), tableCreator(tableCreator)
{

}
