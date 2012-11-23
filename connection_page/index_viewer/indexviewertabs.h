#ifndef INDEXVIEWERTABS_H
#define INDEXVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class IndexViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit IndexViewerTabs(const QString &schemaName,
                             const QString &objectName,
                             DbUiManager *uiManager,
                             QWidget *parent = 0);
    
    virtual void createTabs();
    
};

#endif // INDEXVIEWERTABS_H
