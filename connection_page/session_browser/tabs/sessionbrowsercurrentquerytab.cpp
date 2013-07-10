#include "sessionbrowsercurrentquerytab.h"

SessionBrowserCurrentQueryTab::SessionBrowserCurrentQueryTab(const QString &queryName, DbUiManager *uiManager, SessionBrowserTabs *container, QWidget *parent) :
    SessionBrowserTab(queryName, uiManager, container, parent)
{
}
