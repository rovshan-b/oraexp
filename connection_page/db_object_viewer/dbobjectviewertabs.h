#ifndef DBOBJECTVIEWERTABS_H
#define DBOBJECTVIEWERTABS_H

#include "widgets/subtabwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreemodel.h"

class DbConnection;

class DbObjectViewerTabs : public SubTabWidget
{
    Q_OBJECT
public:
    explicit DbObjectViewerTabs(const QString &schemaName,
                                const QString &objectName,
                                DbTreeModel::DbTreeNodeType itemType,
                                DbUiManager *uiManager,
                                QWidget *parent);

    virtual void createUi();
    virtual void createTabs()=0;

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    IQueryScheduler *scheduler() const;

signals:

public slots:
    void loadTabData(int index);

protected:
    const QString schemaName;
    const QString objectName;
    DbTreeModel::DbTreeNodeType itemType;
    DbUiManager *uiManager;

    IQueryScheduler *queryScheduler;

};

#endif // DBOBJECTVIEWERTABS_H
