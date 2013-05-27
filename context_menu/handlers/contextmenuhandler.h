#ifndef CONTEXTMENUHANDLER_H
#define CONTEXTMENUHANDLER_H

#include <QObject>
#include <QHash>
#include "connection_page/dbuimanager.h"

class ContextMenuHandler
{
public:
    explicit ContextMenuHandler();
    virtual ~ContextMenuHandler(){}
    
    virtual void handle(const QHash<QString,QString> &properties) = 0;

    void setUiManager(DbUiManager *uiManager){this->uiManager = uiManager;}

protected:
    DbUiManager *uiManager;
    
};

#endif // CONTEXTMENUHANDLER_H
