#ifndef TABLECREATORTABWITHTABLEVIEW_H
#define TABLECREATORTABWITHTABLEVIEW_H

#include "connection_page/db_object_creator/dbobjectcreatortabwithtableview.h"
#include "tablecreatortabs.h"

class TableCreatorTabWithTableView : public DbObjectCreatorTabWithTableView<TableCreatorTabs>
{
    Q_OBJECT
public:
    explicit TableCreatorTabWithTableView(TableCreatorTabs *objectCreator, bool editMode, QWidget *parent = 0) :
        DbObjectCreatorTabWithTableView<TableCreatorTabs>(objectCreator, editMode, parent)
    {
        connect(advancedOptionsAction, SIGNAL(toggled(bool)), this, SLOT(showAdvancedOptions(bool)));
    }

protected slots:
    virtual void showAdvancedOptions(bool show)=0;
};

#endif // TABLECREATORTABWITHTABLEVIEW_H
