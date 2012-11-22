#ifndef DBOBJECTREFERENCESVIEWER_H
#define DBOBJECTREFERENCESVIEWER_H

#include "../dbobjectviewergenerictab.h"

class DbObjectReferencesViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectReferencesViewer(bool references, DbUiManager *uiManager, QWidget *parent = 0);
    
protected:
    virtual QList<Param*> getQueryParams();
    
};

#endif // DBOBJECTREFERENCESVIEWER_H
