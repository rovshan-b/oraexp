#ifndef SESSIONBROWSER_H
#define SESSIONBROWSER_H

#include "connection_page/connectionpagetab.h"

class GroupedDataViewWidget;

class SessionBrowser : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit SessionBrowser(DbUiManager *uiManager, QWidget *parent = 0);
    
    virtual void createUi();

    virtual void setConnection(DbConnection *db);
private:
    GroupedDataViewWidget *sessionViewer;
};

#endif // SESSIONBROWSER_H
