#ifndef GENERICOBJECTVIEWERTABS_H
#define GENERICOBJECTVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class GenericObjectViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit GenericObjectViewerTabs(const QString &schemaName,
                                     const QString &objectName,
                                     const QString &dbObjectTypeName,
                                     DbUiManager *uiManager,
                                     QWidget *parent=0);
    
    virtual void createTabs();

private:
    QString dbObjectTypeName;
    
};

#endif // GENERICOBJECTVIEWERTABS_H
