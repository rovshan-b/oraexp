#include "sessionbrowsercurrentquerytab.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

SessionBrowserCurrentQueryTab::SessionBrowserCurrentQueryTab(DbUiManager *uiManager, SessionBrowserTabs *container, QWidget *parent) :
    SessionBrowserTab("get_session_current_query", uiManager, container, parent)
{
}

void SessionBrowserCurrentQueryTab::createMainWidget(QLayout *layout)
{
    editor = new CodeEditorAndSearchPaneWidget();
    editor->setReadOnly(true);

    layout->addWidget(editor);
}

void SessionBrowserCurrentQueryTab::loadData()
{
    queryScheduler->enqueueQuery(this->queryName,
                                 getQueryParams(),
                                 this,
                                 "get_session_current_query",
                                 "sessionQueryCompleted",
                                 "recordFetched",
                                 "fetchCompleted");
}

void SessionBrowserCurrentQueryTab::sessionQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        code = tr("Error retrieving current query");
        queryCompleted();
    }else{
        code = QString();
    }
}

void SessionBrowserCurrentQueryTab::recordFetched(const FetchResult &result)
{
    if(!result.hasError){
        code.append(result.oneRow.at(0));
    }
}

void SessionBrowserCurrentQueryTab::fetchCompleted(const QString &)
{
    editor->editor()->setPlainText(code);

    queryCompleted();
}
