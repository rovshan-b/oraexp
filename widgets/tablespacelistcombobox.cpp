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

    if(queryScheduler){
        comboBox()->loadItems(queryScheduler, "get_tablespace_list");
    }else{
        browseButton()->setEnabled(false);
    }

    browseButton()->setToolTip(tr("Browse tablespaces"));
    connect(this, SIGNAL(buttonClicked(ComboBoxWithButton*)), this, SLOT(detailsButtonClicked()));
}

void TablespaceListComboBox::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_ASSERT(this->queryScheduler==0);

    this->queryScheduler=queryScheduler;
    comboBox()->loadItems(queryScheduler, "get_tablespace_list");
    browseButton()->setEnabled(true);
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
