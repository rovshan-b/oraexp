#include "tablecreatortabwithtableview.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "widgets/datatable.h"
#include "widgets/tabletoolbar.h"
#include "models/genericeditabletablemodel.h"
#include <QtGui>

TableCreatorTabWithTableView::TableCreatorTabWithTableView(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent) :
    TableCreatorTab(tableCreator, editMode, parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    table=new DataTable();
    toolbar=new TableToolbar(table);

    advancedOptionsAction=new QAction(IconUtil::getIcon("advanced_options"), tr("Show/Hide advanced options"), this);
    advancedOptionsAction->setCheckable(true);

    toolbar->addSeparator();
    toolbar->addAction(advancedOptionsAction);

    layout->addWidget(toolbar);
    layout->addWidget(table);

    layout->setContentsMargins(2, 2, 2, 0);

    connect(advancedOptionsAction, SIGNAL(toggled(bool)), this, SLOT(showAdvancedOptions(bool)));

    setLayout(layout);
}

//void TableCreatorTabWithTableView::showAdvancedOptions(bool show)
//{
//}

void TableCreatorTabWithTableView::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(table);
}
