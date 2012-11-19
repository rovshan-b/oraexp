#ifndef VIEWVIEWERTABS_H
#define VIEWVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class ViewViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit ViewViewerTabs(const QString &schemaName,
                            const QString &objectName,
                            DbUiManager *uiManager,
                            QWidget *parent=0);
    
    virtual void createTabs();
    
};

#endif // VIEWVIEWERTABS_H
