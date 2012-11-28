#ifndef DBLINKVIEWER_H
#define DBLINKVIEWER_H

#include "../db_object_viewer/dbobjectviewer.h"

class DbLinkViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit DbLinkViewer(const QString &schemaName,
                          const QString &objectName,
                          DbUiManager *uiManager,
                          QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getViewerWidget() const;
    
};

#endif // DBLINKVIEWER_H
