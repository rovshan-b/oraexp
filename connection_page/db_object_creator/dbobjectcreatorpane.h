#ifndef DBOBJECTCREATORPANE_H
#define DBOBJECTCREATORPANE_H

#include <QWidget>
#include "defines.h"
#include "beans/querylistitem.h"

class IQueryScheduler;

class DbObjectCreatorPane : public QWidget
{
    Q_OBJECT
public:
    explicit DbObjectCreatorPane(const QString &schemaName, const QString &objectName, QWidget *parent = 0);
    
    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QString generateCreateDdl()=0;
    virtual QList< QueryListItem > generateAlterDdl()=0;

    virtual bool beforeCreate() {return true;}
    virtual bool beforeAlter() {return true;}

    virtual QString getSchemaName() const=0;
    virtual QString getObjectName() const=0;

signals:
    void ddlChanged();
    void objectInfoLoaded();
    
protected:
    IQueryScheduler *queryScheduler;

    QString schemaName;
    QString objectName;
};

#endif // DBOBJECTCREATORPANE_H
