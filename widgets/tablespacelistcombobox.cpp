#include "tablespacelistcombobox.h"
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include "util/widgethelper.h"
#include "dialogs/genericresultsetviewerdialog.h"
#include "defines.h"
#include "interfaces/iqueryscheduler.h"
#include "util/queryutil.h"

TablespaceListComboBox::TablespaceListComboBox(IQueryScheduler *queryScheduler,
                                               QWidget *parent) :
    ComboBoxWithButton("", "tablespace", parent), queryScheduler(queryScheduler)
{
    comboBox()->setEditable(true);
    lineEdit()->setMaxLength(MAX_IDENTIFIER_LENGTH);

    comboBox()->loadItems(queryScheduler, "get_tablespace_list");

    browseButton()->setToolTip(tr("Browse tablespaces"));
    connect(this, SIGNAL(buttonClicked(ComboBoxWithButton*)), this, SLOT(detailsButtonClicked()));
}


void TablespaceListComboBox::detailsButtonClicked()
{
    GenericResultsetViewerDialog dialog(queryScheduler, QueryUtil::getQuery("get_tablespace_list_all_cols", queryScheduler->getDb()),
                                        QList<Param*>(), "",this,
                                        qMakePair<QString,QString>("TABLESPACE_NAME", "ICON_NAME"));
    dialog.setWindowTitle(tr("Browse tablespaces"));
    if(dialog.exec()){
        WidgetHelper::setComboBoxText(comboBox(), dialog.selectedText);
    }
}
