#ifndef PAIRCODEVIEWER_H
#define PAIRCODEVIEWER_H

#include "../connectionpagetab.h"
#include "navtree/dbtreemodel.h"
#include "codeviewertabs.h"

class QStackedWidget;

class PairCodeViewer : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit PairCodeViewer(const QString &schemaName,
                            const QString &objectName,
                            DbTreeModel::DbTreeNodeType itemType,
                            DbUiManager *uiManager,
                            QWidget *parent = 0);

    virtual void createUi();

    virtual void setConnection(DbConnection *db);

private slots:
    void switchToSpec();
    void switchToBody();
    
private:
    QStackedWidget *tab;

    CodeViewerTabs *specViewer;
    CodeViewerTabs *bodyViewer;

    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType itemType;

    int initializedCount;
    
};

#endif // PAIRCODEVIEWER_H
