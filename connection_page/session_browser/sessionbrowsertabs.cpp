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
    QIcon noIcon;

    SessionBrowserCurrentQueryTab *currentQueryTab = new SessionBrowserCurrentQueryTab(uiManager, this);
    addTab(currentQueryTab, noIcon, tr("Current query"));

    SessionBrowserTab *waitsTab = new SessionBrowserTab("get_session_waits", uiManager, this);
    addTab(waitsTab, noIcon, tr("Waits"));

    SessionBrowserTab *locksTab = new SessionBrowserTab("get_session_locks", uiManager, this);
    locksTab->setMaxColumnWidth(1024);
    addTab(locksTab, noIcon, tr("Locks"));

    SessionBrowserTab *longopsTab = new SessionBrowserTab("get_session_longops", uiManager, this);
    addTab(longopsTab, noIcon, tr("Long ops"));
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
    if(this->instId != instId || this->sid != sid || this->serial != serial){
        setInstId(instId);
        setSid(sid);
        setSerial(serial);

        refresh();
    }
}

void SessionBrowserTabs::refresh()
{
    for(int i=0; i<tabWidget->count(); ++i){
        static_cast<GenericQueryViewerWidget*>(tabWidget->widget(i))->setNeedsRefresh();
    }

    refreshInfo();
}
