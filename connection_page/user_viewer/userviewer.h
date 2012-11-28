#ifndef USERVIEWER_H
#define USERVIEWER_H

#include "connection_page/db_object_viewer/dbobjectviewer.h"

class UserViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit UserViewer(const QString &schemaName,
                        const QString &objectName,
                        DbUiManager *uiManager,
                        QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getViewerWidget() const;
    
};

#endif // USERVIEWER_H
