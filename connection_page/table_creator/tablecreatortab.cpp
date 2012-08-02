#include "tablecreatortab.h"
#include "connectivity/dbconnection.h"

TableCreatorTab::TableCreatorTab(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent) :
    QWidget(parent), db(0), tableCreator(tableCreator), editMode(editMode)
{

}
