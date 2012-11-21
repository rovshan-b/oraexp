#include "dbobjectddlviewer.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "connectivity/dbconnection.h"
#include "util/queryexectask.h"
#include "util/queryutil.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "dialogs/tableddloptionsdialog.h"
#include <QtGui>

DbObjectDdlViewer::DbObjectDdlViewer(bool addSettingsButton, QWidget *parent) :
    DbObjectViewerGenericTab("", parent),
    addSettingsButton(addSettingsButton)
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
    editor=new CodeEditorAndSearchPaneWidget();
    editor->setReadOnly(true);

    layout->addWidget(editor);
}

void DbObjectDdlViewer::loadData()
{
    editor->editor()->clear();

    QList<Param*> params;
    if(itemType==DbTreeModel::Table){
        queryName="get_object_ddl";
        params.append(ddlOptions.getQueryParams());
    }else{
        queryName="get_object_source_code";
        params = QList<Param*>() <<
                                    new Param("target_owner", this->schemaName) <<
                                    new Param("wrap", 0) <<
                                    new Param("sql_terminator", 1);
    }
    params.append(new Param(":object_type", DbUtil::getDbObjectTypeNameByNodeType(this->itemType)));
    params.append(getQueryParams());

    queryScheduler->enqueueQuery(queryName,
                                 params,
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
        if(result.exception.getErrorCode()==31603){
            editor->setInitialText(tr("--Body does not exist or you do not have permission to view it."));
        }else{
            QMessageBox::critical(this->window(), tr("Error retrieving table DDL"), result.exception.getErrorMessage());
        }

        queryCompleted();
    }
}

void DbObjectDdlViewer::ddlFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching table DDL"), fetchResult.exception.getErrorMessage());
    }else{
        editor->setInitialText(fetchResult.oneRow.at(0));
    }
}

void DbObjectDdlViewer::fetchCompleted(const QString &)
{
    queryCompleted();
}

QList<QAction*> DbObjectDdlViewer::getSpecificToolbarButtons()
{
    QList<QAction*> list;

    if(addSettingsButton){
        QAction *separator=new QAction(this);
        separator->setSeparator(true);
        list.append(separator);

        QAction *settingsAction=new QAction(IconUtil::getIcon("settings"), tr("DDL options"), this);
        connect(settingsAction, SIGNAL(triggered()), this, SLOT(showDdlOptions()));
        list.append(settingsAction);
    }

    return list;
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
