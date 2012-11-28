#ifndef DBLINKVIEWERTABS_H
#define DBLINKVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class DbLinkViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit DbLinkViewerTabs(const QString &schemaName,
                              const QString &objectName,
                              DbUiManager *uiManager,
                              QWidget *parent=0);
    
    virtual void createTabs();
    
};

#endif // DBLINKVIEWERTABS_H
