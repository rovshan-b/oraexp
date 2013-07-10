#include "dbobjectviewergenerictab.h"
#include "util/dbutil.h"

DbObjectViewerGenericTab::DbObjectViewerGenericTab(const QString &queryName, DbUiManager *uiManager, QWidget *parent) :
    GenericQueryViewerWidget(queryName, uiManager, parent),
    setObjectNameParam(true),
    setObjectTypeParam(false)
{
}

DbObjectViewerGenericTab::~DbObjectViewerGenericTab()
{

}

void DbObjectViewerGenericTab::setObjectName(const QString &schemaName, const QString &objectName, DbTreeModel::DbTreeNodeType itemType)
{
    Q_ASSERT(this->schemaName.isEmpty());

    this->schemaName=schemaName;
    this->objectName=objectName;
    this->itemType=itemType;
}



QList<Param *> DbObjectViewerGenericTab::getQueryParams()
{
    QList<Param*> queryParams;

    queryParams.append(new Param(":owner", schemaName));

    if(setObjectNameParam){
        queryParams.append(new Param(":object_name", objectName));
    }

    if(setObjectTypeParam){
        queryParams.append(new Param(":object_type", DbUtil::getDbObjectTypeNameByNodeType(itemType)));
    }

    return queryParams;
}
