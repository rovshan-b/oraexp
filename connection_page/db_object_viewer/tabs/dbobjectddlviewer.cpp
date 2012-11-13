#include "dbobjectddlviewer.h"
#include "codeeditor/codeeditor.h"
#include "connectivity/dbconnection.h"
#include "util/queryexectask.h"
#include "util/queryutil.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "dialogs/tableddloptionsdialog.h"
#include <QtGui>

DbObjectDdlViewer::DbObjectDdlViewer(DbTreeModel::DbTreeNodeType itemType, QWidget *parent) :
    DbObjectViewerGenericTab("", parent),
    itemType(itemType)
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

void DbObjectDdlViewer::createMainWidget(QLayout *layout)
{
    editor=new CodeEditor();
    editor->setReadOnly(true);

    layout->addWidget(editor);
}

void DbObjectDdlViewer::loadData()
{
    editor->setPlainText("");

    queryScheduler->enqueueQuery("get_object_ddl",
                                 QList<Param*>() <<
                                 new Param(":owner", this->schemaName) <<
                                 new Param(":object_name", this->objectName) <<
                                 new Param(":object_type", DbUtil::getDbObjectTypeNameByNodeType(this->itemType)) <<
                                 ddlOptions.getQueryParams(),
                                 this,
                                 "get_object_ddl",
                                 "ddlQueryCompleted",
                                 "ddlFetched",
                                 "fetchCompleted",
                                 true);
}

void DbObjectDdlViewer::ddlQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving table DDL"), result.exception.getErrorMessage());
        queryCompleted();
    }
}

void DbObjectDdlViewer::ddlFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching table DDL"), fetchResult.exception.getErrorMessage());
    }else{
        editor->setPlainText(fetchResult.oneRow.at(0));
    }
}

void DbObjectDdlViewer::fetchCompleted(const QString &)
{
    queryCompleted();
}

void DbObjectDdlViewer::addSpecificToolbarButtons()
{
    toolbar->addSeparator();
    toolbar->addAction(IconUtil::getIcon("settings"), tr("DDL options"), this, SLOT(showDdlOptions()));
}

void DbObjectDdlViewer::showDdlOptions()
{
    TableDdlOptionsDialog dialog(this);
    dialog.setOptions(ddlOptions);
    if(dialog.exec()){
        ddlOptions=dialog.getOptions();
        refreshInfo();
    }
}
