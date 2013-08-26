#ifndef GENERICOBJECTVIEWER_H
#define GENERICOBJECTVIEWER_H

#include "connection_page/db_object_viewer/dbobjectviewer.h"

class GenericObjectViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit GenericObjectViewer(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getViewerWidget() const;
};

#endif // GENERICOBJECTVIEWER_H
