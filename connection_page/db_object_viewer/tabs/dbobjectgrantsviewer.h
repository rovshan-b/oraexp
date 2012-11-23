#ifndef DBOBJECTGRANTSVIEWER_H
#define DBOBJECTGRANTSVIEWER_H

#include "../dbobjectviewergenerictab.h"

class DbObjectGrantsViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectGrantsViewer(DbUiManager *uiManager, QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // DBOBJECTGRANTSVIEWER_H
