#ifndef VIEWVIEWER_H
#define VIEWVIEWER_H

#include "../db_object_viewer/dbobjectviewer.h"

class ViewViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit ViewViewer(const QString &schemaName,
                        const QString &objectName,
                        DbUiManager *uiManager,
                        QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getTabsWidget() const;
    
};

#endif // VIEWVIEWER_H
