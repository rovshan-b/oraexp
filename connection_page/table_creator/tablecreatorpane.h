#ifndef TABLECREATORPANE_H
#define TABLECREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorpane.h"

class TableCreatorTabs;

class TableCreatorPane : public DbObjectCreatorPane
{
    Q_OBJECT
public:
    explicit TableCreatorPane(const QString &schemaName, const QString &tableName, QWidget *parent = 0);
    
    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QString generateCreateDdl();
    virtual QList< QueryListItem > generateAlterDdl();

    //virtual bool beforeCreate();
    virtual bool beforeAlter();

    QString getSchemaName() const;
    QString getObjectName() const;

private:
    TableCreatorTabs *tableCreatorTabs;
    
};

#endif // TABLECREATORPANE_H
