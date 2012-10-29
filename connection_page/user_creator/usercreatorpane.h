#ifndef USERCREATORPANE_H
#define USERCREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorpane.h"

class UserCreatorTabs;

class UserCreatorPane : public DbObjectCreatorPane
{
    Q_OBJECT
public:
    explicit UserCreatorPane(DbObjectCreator *objectCreator, QWidget *parent = 0);
    
    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QString generateCreateDdl();
    virtual QList< QueryListItem > generateAlterDdl();

    virtual bool beforeCreate() const;
    virtual bool beforeAlter() const;

    virtual QString getSchemaName() const;
    virtual QString getObjectName() const;
    
private:
    UserCreatorTabs *userCreatorTabs;
};

#endif // USERCREATORPANE_H
