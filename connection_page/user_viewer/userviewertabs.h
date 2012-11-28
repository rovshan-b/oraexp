#ifndef USERVIEWERTABS_H
#define USERVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class UserViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit UserViewerTabs(const QString &schemaName,
                            const QString &objectName,
                            DbUiManager *uiManager,
                            QWidget *parent=0);
    
    virtual void createTabs();
    
};

#endif // USERVIEWERTABS_H
