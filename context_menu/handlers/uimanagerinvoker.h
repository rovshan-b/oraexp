#ifndef UIMANAGERINVOKER_H
#define UIMANAGERINVOKER_H

#include "contextmenuhandler.h"

class UiManagerInvoker : public ContextMenuHandler
{
public:
    UiManagerInvoker();

    virtual void handle(const QHash<QString,QString> &properties);
};

#endif // UIMANAGERINVOKER_H
