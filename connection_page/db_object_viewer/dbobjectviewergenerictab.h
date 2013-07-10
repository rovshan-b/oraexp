#ifndef DBOBJECTVIEWERGENERICTAB_H
#define DBOBJECTVIEWERGENERICTAB_H

#include "widgets/genericqueryviewerwidget.h"
#include "connectivity/queryresult.h"
#include "util/param.h"
#include "navtree/dbtreemodel.h"
#include "interfaces/iqueryscheduler.h"

class DataTable;
class StatementDesc;
class DbUiManager;

class DbObjectViewerGenericTab : public GenericQueryViewerWidget
{
    Q_OBJECT
public:
    explicit DbObjectViewerGenericTab(const QString &queryName, DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~DbObjectViewerGenericTab();

    virtual void setObjectName(const QString &schemaName,
                               const QString &objectName,
                               DbTreeModel::DbTreeNodeType itemType);



    void setUseObjectNameParam(bool use){this->setObjectNameParam=use;}
    void setUseObjectTypeParam(){this->setObjectTypeParam=true;}


protected:
    virtual QList<Param*> getQueryParams();

    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType itemType;

    bool setObjectNameParam;
    bool setObjectTypeParam;

};

#endif // DBOBJECTVIEWERGENERICTAB_H
