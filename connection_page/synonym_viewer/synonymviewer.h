#ifndef SYNONYMVIEWER_H
#define SYNONYMVIEWER_H

#include "../db_object_viewer/dbobjectviewer.h"

class SynonymViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit SynonymViewer(const QString &schemaName,
                           const QString &objectName,
                           DbUiManager *uiManager,
                           QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getViewerWidget() const;
    
};

#endif // SYNONYMVIEWER_H
