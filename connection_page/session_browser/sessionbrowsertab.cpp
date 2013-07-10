#include "sessionbrowsertab.h"
#include "sessionbrowsertabs.h"

SessionBrowserTab::SessionBrowserTab(const QString &queryName, DbUiManager *uiManager, SessionBrowserTabs *container, QWidget *parent) :
    GenericQueryViewerWidget(queryName, uiManager, parent),
    container(container)
{
}

QList<Param *> SessionBrowserTab::getQueryParams()
{
    return container->getQueryParams();
}
