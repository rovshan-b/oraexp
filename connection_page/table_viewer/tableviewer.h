#ifndef TABLEVIEWER_H
#define TABLEVIEWER_H

#include "connection_page/db_object_viewer/dbobjectviewer.h"

class TableViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit TableViewer(const QString &schemaName,
                         const QString &objectName,
                         DbUiManager *uiManager,
                         QWidget *parent = 0);

    virtual DbObjectViewerTabs *getViewerWidget() const;
    
signals:
    
public slots:
    
};

#endif // TABLEVIEWER_H
