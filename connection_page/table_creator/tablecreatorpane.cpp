#include "tablecreatorpane.h"
#include "tablecreatortabs.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"
#include "tablecreatortabwithtableview.h"
#include "models/genericeditabletablemodel.h"
#include <QtGui>

TableCreatorPane::TableCreatorPane(DbObjectCreator *objectCreator, bool editMode, QWidget *parent) :
    DbObjectCreatorPane(objectCreator, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);

    tableCreatorTabs=new TableCreatorTabs(objectCreator->getOriginalSchemaName(),
                                          objectCreator->getOriginalObjectName(),
                                          editMode);

    mainLayout->addWidget(tableCreatorTabs);
    setLayout(mainLayout);

    connect(tableCreatorTabs, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(tableCreatorTabs, SIGNAL(objectInfoLoaded()), this, SIGNAL(objectInfoLoaded()));
    connect(tableCreatorTabs, SIGNAL(objectInfoLoaded()), this, SLOT(highlightChildObject()));

    switchToRequestedTab();
}

void TableCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorPane::setQueryScheduler(queryScheduler);

    tableCreatorTabs->setQueryScheduler(queryScheduler);
}

QString TableCreatorPane::generateCreateDdl()
{
    return tableCreatorTabs->getTableInfo().generateDdl();
}

QList<QueryListItem> TableCreatorPane::generateAlterDdl()
{
    return tableCreatorTabs->generateAlterDdl();
}

bool TableCreatorPane::beforeAlter()
{
    tableCreatorTabs->removeIncorrectRows();
    return tableCreatorTabs->beforeAlterTable();
}

QString TableCreatorPane::getSchemaName() const
{
    return tableCreatorTabs->getSchemaName();
}

QString TableCreatorPane::getObjectName() const
{
    return tableCreatorTabs->getTableName();
}

QWidget *TableCreatorPane::switchToRequestedTab()
{
    QString tabName = getObjectCreator()->propertyValue(ConnectionPageTab::TAB_NAME_KEY);

    if(tabName.isEmpty()){
        return 0;
    }

    QWidget *tab = 0;
    for(int i=0; i < tableCreatorTabs->count(); ++i){
        QWidget *widget = tableCreatorTabs->widget(i);
        if(widget->objectName() == tabName){
            tableCreatorTabs->setCurrentWidget(widget);
            tab = widget;
            break;
        }
    }

    return tab;
}

void TableCreatorPane::highlightChildObject()
{
    QWidget *tab = switchToRequestedTab();

    if(tab == 0){
        return;
    }

    TableCreatorTabWithTableView *childObjectCreatorTab = qobject_cast<TableCreatorTabWithTableView*>(tab);
    if(childObjectCreatorTab == 0){
        return;
    }

    DataTable *table = childObjectCreatorTab->getTable();
    GenericEditableTableModel *model = qobject_cast<GenericEditableTableModel*>(table->model());

    if(!model){
        return;
    }

    QString childObjectName = getObjectCreator()->propertyValue(ConnectionPageTab::CHILD_OBJECT_NAME_KEY);
    int titleColumnIx = model->getTitleColumn();

    if(model->rowCount() == 0){
        model->ensureRowCount(1);
        return;
    }

    int lastRowIx = model->rowCount()-1;
    int selectedRowIx;

    if(childObjectName.isEmpty()){ //ensure last row is selected. if last row contains some data append new row
        QString titleColumnValue = model->index(lastRowIx, titleColumnIx).data().toString().trimmed();
        if(titleColumnValue.isEmpty()){
            selectedRowIx = lastRowIx;
        }else{
            model->ensureRowCount(model->rowCount() + 1);
            selectedRowIx = model->rowCount() - 1;
        }
        table->setCurrentIndex(model->index(selectedRowIx, titleColumnIx));
        table->scrollTo(model->index(selectedRowIx, titleColumnIx));
    }else{
        QModelIndex matchingIndex = model->findItem(titleColumnIx, QRegExp(childObjectName));
        if(matchingIndex.isValid()){
            table->setCurrentIndex(matchingIndex);
            table->scrollTo(matchingIndex, QAbstractItemView::PositionAtCenter);
        }
    }


}
