#include "tableddlviewer.h"
#include "codeeditor/codeeditor.h"
#include "connectivity/dbconnection.h"
#include "../tableinfotoolbar.h"
#include "util/queryexectask.h"
#include "util/queryutil.h"
#include "util/iconutil.h"
#include "dialogs/tableddloptionsdialog.h"
#include <QtGui>

TableDdlViewer::TableDdlViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{
    ddlOptions.sqlTerminators="y";
    ddlOptions.segmentAttributes="n";
    ddlOptions.storage="n";
    ddlOptions.tablespace="n";
    ddlOptions.constraints="y";
    ddlOptions.constraintsAsAlter="n";
    ddlOptions.refConstraints="y";
    ddlOptions.oid="n";
    ddlOptions.sizeByteKeyword="n";
}

void TableDdlViewer::createMainWidget(QLayout *layout)
{
    editor=new CodeEditor();
    editor->setReadOnly(true);

    layout->addWidget(editor);
}

void TableDdlViewer::loadData()
{
    editor->setPlainText("");

    queryScheduler->enqueueQuery("get_object_ddl",
                                 QList<Param*>() <<
                                 new Param(":owner", this->schemaName) <<
                                 new Param(":object_name", this->tableName) <<
                                 new Param(":object_type", QString("TABLE")) <<
                                 ddlOptions.getQueryParams(),
                                 this,
                                 "get_table_ddl",
                                 "ddlQueryCompleted",
                                 "ddlFetched",
                                 "fetchCompleted",
                                 true);
}

void TableDdlViewer::ddlQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving table DDL"), result.exception.getErrorMessage());
        queryCompleted();
    }
}

void TableDdlViewer::ddlFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching table DDL"), fetchResult.exception.getErrorMessage());
    }else{
        editor->setPlainText(fetchResult.oneRow.at(0));
    }
}

void TableDdlViewer::fetchCompleted(const QString &)
{
    queryCompleted();
}

void TableDdlViewer::addSpecificToolbarButtons()
{
    toolbar->addSeparator();
    toolbar->addAction(IconUtil::getIcon("settings"), tr("DDL options"), this, SLOT(showDdlOptions()));
}

void TableDdlViewer::showDdlOptions()
{
    TableDdlOptionsDialog dialog(this);
    dialog.setOptions(ddlOptions);
    if(dialog.exec()){
        ddlOptions=dialog.getOptions();
        refreshInfo();
    }
}
