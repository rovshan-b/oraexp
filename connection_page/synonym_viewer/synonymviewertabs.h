#ifndef SYNONYMVIEWERTABS_H
#define SYNONYMVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class SynonymViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit SynonymViewerTabs(const QString &schemaName,
                               const QString &objectName,
                               DbUiManager *uiManager,
                               QWidget *parent=0);
    
    virtual void createTabs();
    
};

#endif // SYNONYMVIEWERTABS_H
