#include "sessionbrowsertabs.h"
#include "widgets/genericqueryviewerwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "tabs/sessionbrowsercurrentquerytab.h"
#include <QtGui>

SessionBrowserTabs::SessionBrowserTabs(DbUiManager *uiManager, QWidget *parent) :
    GenericQueryViewerTabs(uiManager, parent),
    instId(-1),
    sid(-1)
{
}

void SessionBrowserTabs::createTabs()
{
    SessionBrowserCurrentQueryTab *currentQueryTab = new SessionBrowserCurrentQueryTab("get_session_current_query",
                                                                                       uiManager,
                                                                                       this);
    addTab(currentQueryTab, IconUtil::getIcon("query"), tr("Current query"));
}

QList<Param *> SessionBrowserTabs::getQueryParams()
{
    QList<Param*> params;
    params.append(new Param("sid", sid));

    if(queryScheduler->getDb()->supportsGlobalPerformanceViews()){
        params.append(new Param("inst_id", instId));
    }

    return params;
}

void SessionBrowserTabs::setCurrentSession(int instId, int sid, int serial)
{
    setInstId(instId);
    setSid(sid);
    setSerial(serial);

    refresh();
}

void SessionBrowserTabs::refresh()
{
    for(int i=0; i<tabWidget->count(); ++i){
        static_cast<GenericQueryViewerWidget*>(tabWidget->widget(i))->setNeedsRefresh();
    }

    refreshInfo();
}
