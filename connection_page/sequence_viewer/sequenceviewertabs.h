#ifndef SEQUENCEVIEWERTABS_H
#define SEQUENCEVIEWERTABS_H

#include "connection_page/db_object_viewer/dbobjectviewertabs.h"

class SequenceViewerTabs : public DbObjectViewerTabs
{
    Q_OBJECT
public:
    explicit SequenceViewerTabs(const QString &schemaName,
                                const QString &objectName,
                                DbUiManager *uiManager,
                                QWidget *parent=0);
    
    virtual void createTabs();
    
};

#endif // SEQUENCEVIEWERTABS_H
