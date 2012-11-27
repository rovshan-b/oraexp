#ifndef SEQUENCEVIEWER_H
#define SEQUENCEVIEWER_H

#include "connection_page/db_object_viewer/dbobjectviewer.h"

class SequenceViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit SequenceViewer(const QString &schemaName,
                            const QString &objectName,
                            DbUiManager *uiManager,
                            QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getViewerWidget() const;
    
};

#endif // SEQUENCEVIEWER_H
