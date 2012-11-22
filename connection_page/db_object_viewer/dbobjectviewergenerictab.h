#ifndef DBOBJECTVIEWERGENERICTAB_H
#define DBOBJECTVIEWERGENERICTAB_H

#include <QWidget>
#include "widgets/ondemandinfoviewerwidget.h"
#include "connectivity/queryresult.h"
#include "util/param.h"
#include "navtree/dbtreemodel.h"
#include "interfaces/iqueryscheduler.h"

class DataTable;
class StatementDesc;
class DbUiManager;

class DbObjectViewerGenericTab : public OnDemandInfoViewerWidget
{
    Q_OBJECT
public:
    explicit DbObjectViewerGenericTab(const QString &queryName, DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~DbObjectViewerGenericTab();

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    virtual void setObjectName(const QString &schemaName,
                               const QString &objectName,
                               DbTreeModel::DbTreeNodeType itemType);

    virtual void createMainWidget(QLayout *);

    void createChildControls();
    bool areControlsCreated() const {return this->controlsCreated;}

    virtual QList<QAction*> getSpecificToolbarButtons();

    virtual void loadData();

    void setIconColumn(const QString &displayColumnName, const QString &iconColumnName);
    void setDynamicQuery(int colNum, StatementDesc *stmtDesc);

    void setUseObjectTypeParam(){this->setObjectTypeParam=true;}

    void setObjectListMode(int schemaNameCol,
                           int objectNameCol,
                           int objectTypeCol,
                           const QString &objectListSchemaName=QString(),
                           const QString &objectListObjectType=QString());

protected slots:
    void queryCompleted();

protected:
    virtual void doLoadInfo();
    virtual QList<Param*> getQueryParams();

    DbUiManager *uiManager;
    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType itemType;

    bool controlsCreated;

    bool setObjectTypeParam;

    DataTable *dt;
    QString queryName;
    QString query;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<QString, QString> iconColumns;

    QString objectListSchemaName;
    QString objectListObjectType;
    int schemaNameCol;
    int objectNameCol;
    int objectTypeCol;

};

#endif // DBOBJECTVIEWERGENERICTAB_H
