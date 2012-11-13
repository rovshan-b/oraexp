#ifndef TABLEVIEWERTABS_H
#define TABLEVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class TableViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit TableViewerTabs(const QString &schemaName,
                             const QString &objectName,
                             QWidget *parent=0);
    
    virtual void createTabs();
};

#endif // TABLEVIEWERTABS_H
