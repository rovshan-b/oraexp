#ifndef TABLEINFOGENERICTAB_H
#define TABLEINFOGENERICTAB_H

#include <QWidget>
#include "tableinfoviewerwidget.h"
#include "connectivity/queryresult.h"
#include "util/param.h"

class DataTable;
class StatementDesc;

class TableInfoGenericTab : public TableInfoViewerWidget
{
    Q_OBJECT
public:
    explicit TableInfoGenericTab(QWidget *parent = 0);
    virtual ~TableInfoGenericTab();

    virtual void createMainWidget(QLayout *);

    virtual void loadData();

protected slots:
    void queryCompleted();

protected:
    virtual void doLoadInfo();
    virtual QList<Param*> getQueryParams();

    DataTable *dt;
    QString query;
    QHash<int, StatementDesc*> dynamicQueries;
    //bool retrieveResultsetAsBindParameter;

};

#endif // TABLEINFOGENERICTAB_H
