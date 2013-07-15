#ifndef CONTEXTMENUACTIONCREATOR_H
#define CONTEXTMENUACTIONCREATOR_H

#include <QIcon>
#include <QString>

class DynamicAction;
class DbUiManager;

class ContextMenuActionCreator
{
public:
    ContextMenuActionCreator(DbUiManager *uiManager);

    virtual DynamicAction *create(const QIcon &icon, const QString &caption, QObject *parent) = 0;

protected:
    DbUiManager *uiManager;
};

#endif // CONTEXTMENUACTIONCREATOR_H
