#ifndef DBOBJECTCREATORPANE_H
#define DBOBJECTCREATORPANE_H

#include <QWidget>
#include "defines.h"
#include "beans/querylistitem.h"

class IQueryScheduler;
class DbObjectCreator;

class DbObjectCreatorPane : public QWidget
{
    Q_OBJECT
public:
    explicit DbObjectCreatorPane(DbObjectCreator *objectCreator, QWidget *parent = 0);
    
    //inheriting classes can override this method to change ui creation logic
    virtual void createUi(){}

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QString generateCreateDdl()=0;
    virtual QList< QueryListItem > generateAlterDdl()=0;

    virtual bool beforeCreate() const {return true;}
    virtual bool beforeAlter() const {return true;}

    virtual QString getSchemaName() const=0;
    virtual QString getObjectName() const=0;

    virtual void highlightChildObject() {}

    DbObjectCreator *getObjectCreator() const {return this->objectCreator;}

signals:
    void ddlChanged();
    void objectInfoLoaded();
    
protected:
    DbObjectCreator *objectCreator;
    IQueryScheduler *queryScheduler;
};

#endif // DBOBJECTCREATORPANE_H
