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
    
    virtual void handle(DbUiManager *uiManager, const QHash<QString,QString> &properties) = 0;
    
};

#endif // CONTEXTMENUHANDLER_H
