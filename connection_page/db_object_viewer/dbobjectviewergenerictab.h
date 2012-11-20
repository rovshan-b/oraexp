#ifndef DBOBJECTVIEWERGENERICTAB_H
#define DBOBJECTVIEWERGENERICTAB_H

#include <QWidget>
#include "dbobjectviewerwidget.h"
#include "connectivity/queryresult.h"
#include "util/param.h"

class DataTable;
class StatementDesc;

class DbObjectViewerGenericTab : public DbObjectViewerWidget
{
    Q_OBJECT
public:
    explicit DbObjectViewerGenericTab(const QString &queryName, QWidget *parent = 0);
    virtual ~DbObjectViewerGenericTab();

    virtual void createMainWidget(QLayout *);

    virtual void loadData();

    void setIconColumn(const QString &displayColumnName, const QString &iconColumnName);
    void setDynamicQuery(int colNum, StatementDesc *stmtDesc);

    void setUseObjectTypeParam(){this->setObjectTypeParam=true;}

protected slots:
    void queryCompleted();

protected:
    virtual void doLoadInfo();
    virtual QList<Param*> getQueryParams();

    bool setObjectTypeParam;

    DataTable *dt;
    QString queryName;
    QString query;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<QString, QString> iconColumns;

};

#endif // DBOBJECTVIEWERGENERICTAB_H
