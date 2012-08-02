#ifndef TABLECREATORTABWITHTABLEVIEW_H
#define TABLECREATORTABWITHTABLEVIEW_H

#include "tablecreatortab.h"

class DataTable;
class QToolBar;
class QAction;

class TableCreatorTabWithTableView : public TableCreatorTab
{
    Q_OBJECT

public:
    explicit TableCreatorTabWithTableView(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent = 0);

    virtual void removeIncorrectRows();

protected slots:
    virtual void showAdvancedOptions(bool show)=0;

protected:
    DataTable *table;
    QToolBar *toolbar;

private:
    QAction* advancedOptionsAction;

};

#endif // TABLECREATORTABWITHTABLEVIEW_H
