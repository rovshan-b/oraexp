#ifndef PAIRCODEVIEWER_H
#define PAIRCODEVIEWER_H

#include "../connectionpagetab.h"
#include "navtree/dbtreemodel.h"

class PairCodeViewer : public ConnectionPageTab
{
    Q_OBJECT
public:
    //1 - DbObjectViewer-den DbObjectViewerWidget refactor elemek, hansi ki, tekce QWidget-den inherit etmelidir
    //ve setConnection yox setQueryScheduler metodu olmalidir (indiki DbObjectViewerWidget-in adini deyishmek lazimdir)
    //2 - 1-cini edenden sonra ele etmek lazimdir ki, bu class 2 eded DbObjectViewerWidget yaradib onlardan istifade elesin

    explicit PairCodeViewer(const QString &schemaName,
                            const QString &objectName,
                            DbTreeModel::DbTreeNodeType itemType,
                            DbUiManager *uiManager,
                            QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // PAIRCODEVIEWER_H
