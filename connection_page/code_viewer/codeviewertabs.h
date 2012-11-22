#ifndef CODEVIEWERTABS_H
#define CODEVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class CodeViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit CodeViewerTabs(const QString &schemaName,
                            const QString &objectName,
                            DbTreeModel::DbTreeNodeType itemType,
                            DbUiManager *uiManager,
                            QWidget *parent=0);
    
    virtual void createTabs();


};

#endif // CODEVIEWERTABS_H
