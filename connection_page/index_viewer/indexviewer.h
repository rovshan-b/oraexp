#ifndef INDEXVIEWER_H
#define INDEXVIEWER_H

#include "../db_object_viewer/dbobjectviewer.h"

class IndexViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit IndexViewer(const QString &schemaName,
                         const QString &objectName,
                         DbUiManager *uiManager,
                         QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getViewerWidget() const;
    
};

#endif // INDEXVIEWER_H
