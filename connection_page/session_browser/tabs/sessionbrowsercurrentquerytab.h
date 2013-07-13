#ifndef SESSIONBROWSERCURRENTQUERYTAB_H
#define SESSIONBROWSERCURRENTQUERYTAB_H

#include "../sessionbrowsertab.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class CodeEditorAndSearchPaneWidget;

class SessionBrowserCurrentQueryTab : public SessionBrowserTab
{
    Q_OBJECT
public:
    explicit SessionBrowserCurrentQueryTab(DbUiManager *uiManager,
                                           SessionBrowserTabs *container,
                                           QWidget *parent = 0);
    
    virtual void createMainWidget(QLayout *layout);

    virtual void loadData();

private slots:
    void sessionQueryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);

private:
    CodeEditorAndSearchPaneWidget *editor;

    QString code;
    
};

#endif // SESSIONBROWSERCURRENTQUERYTAB_H
