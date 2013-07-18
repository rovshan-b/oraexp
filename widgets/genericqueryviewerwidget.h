#ifndef GENERICQUERYVIEWERWIDGET_H
#define GENERICQUERYVIEWERWIDGET_H

#include "ondemandinfoviewerwidget.h"
#include <QHash>

class IQueryScheduler;
class DbUiManager;
class DataTable;
class Param;
class StatementDesc;

class GenericQueryViewerWidget : public OnDemandInfoViewerWidget
{
    Q_OBJECT
public:
    explicit GenericQueryViewerWidget(const QString &queryName, DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~GenericQueryViewerWidget();

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    
    virtual void createMainWidget(QLayout *);

    void createChildControls();
    bool areControlsCreated() const {return this->controlsCreated;}

    virtual QList<QAction*> getSpecificToolbarButtons();

    virtual void loadData();

    void setIconColumn(const QString &displayColumnName, const QString &iconColumnName);
    void setDynamicQuery(int colNum, StatementDesc *stmtDesc);

    void setObjectListMode(int schemaNameCol,
                           int objectNameCol,
                           int objectTypeCol,
                           const QString &objectListSchemaName=QString(),
                           const QString &objectListObjectType=QString());

    void setMaxColumnWidth(int maxColumnWidth);

protected slots:
    void queryCompleted();

protected:
    virtual void doLoadInfo();
    virtual QList<Param*> getQueryParams() = 0;

    IQueryScheduler *queryScheduler;

    QString queryName;
    DbUiManager *uiManager;
    bool controlsCreated;

    DataTable *dt;

    QString query;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<QString, QString> iconColumns;

    QString objectListSchemaName;
    QString objectListObjectType;
    int schemaNameCol;
    int objectNameCol;
    int objectTypeCol;

private:
    int maxColumnWidth;
    
};

#endif // GENERICQUERYVIEWERWIDGET_H
