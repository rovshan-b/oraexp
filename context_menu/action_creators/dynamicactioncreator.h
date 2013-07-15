#ifndef DYNAMICACTIONCREATOR_H
#define DYNAMICACTIONCREATOR_H

#include "contextmenuactioncreator.h"
#include <QHash>

class DynamicActionCreator : public ContextMenuActionCreator
{
public:
    DynamicActionCreator(DbUiManager *uiManager, const QHash<QString,QString> &properties);

    QHash<QString,QString> properties;

    virtual DynamicAction *create(const QIcon &icon, const QString &caption, QObject *parent);
};

#endif // DYNAMICACTIONCREATOR_H
