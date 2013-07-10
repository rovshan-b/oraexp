#ifndef SESSIONBROWSERCURRENTQUERYTAB_H
#define SESSIONBROWSERCURRENTQUERYTAB_H

#include "../sessionbrowsertab.h"

class SessionBrowserCurrentQueryTab : public SessionBrowserTab
{
    Q_OBJECT
public:
    explicit SessionBrowserCurrentQueryTab(const QString &queryName,
                                           DbUiManager *uiManager,
                                           SessionBrowserTabs *container,
                                           QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // SESSIONBROWSERCURRENTQUERYTAB_H
