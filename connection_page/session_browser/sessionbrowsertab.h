#ifndef SESSIONBROWSERTAB_H
#define SESSIONBROWSERTAB_H

#include "widgets/genericqueryviewerwidget.h"

class SessionBrowserTabs;

class SessionBrowserTab : public GenericQueryViewerWidget
{
    Q_OBJECT
public:
    explicit SessionBrowserTab(const QString &queryName,
                               DbUiManager *uiManager,
                               SessionBrowserTabs *container,
                               QWidget *parent = 0);
    
protected:
    virtual QList<Param*> getQueryParams();

private:
    SessionBrowserTabs *container;
    
};

#endif // SESSIONBROWSERTAB_H
