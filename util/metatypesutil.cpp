#include "metatypesutil.h"

#include "connectivity/dbconnection.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "connectivity/ociexception.h"
#include "navtree/dbtreeitemresult.h"
#include "navtree/dbtreemodel.h"
#include "util/queryexectask.h"
#include "beans/datacompareinfo.h"

#include "beans/columninfo.h"
#include "beans/constraintinfo.h"
#include "beans/indexinfo.h"
#include "beans/triggerinfo.h"
#include "beans/objectgrantinfo.h"
#include "beans/tableexternalinfo.h"

MetaTypesUtil::MetaTypesUtil()
{
}

void MetaTypesUtil::registerMetaTypes()
{
    qRegisterMetaType<DbConnection>("DbConnection");
    qRegisterMetaTypeStreamOperators<DbConnection>("DbConnection");

    qRegisterMetaType<QueryResult>("QueryResult");
    qRegisterMetaType<FetchResult>("FetchResult");
    qRegisterMetaType<DbTreeItemResult>("DbTreeItemResult");
    qRegisterMetaType<OciException>("OciException");

    qRegisterMetaType<QueryExecTask>("QueryExecTask");
    qRegisterMetaType< QList<QStringList> >("QList<QStringList>");

    qRegisterMetaType<DataCompareInfo>("DataCompareInfo");

    qRegisterMetaType< QList<ColumnInfo> >("QList<ColumnInfo>");
    qRegisterMetaType< QList<ConstraintInfo> >("QList<ConstraintInfo>");
    qRegisterMetaType< QList<IndexInfo> >("QList<IndexInfo>");
    qRegisterMetaType< QList<TriggerInfo> >("QList<TriggerInfo>");
    qRegisterMetaType< QList<ObjectGrantInfo> >("QList<ObjectGrantInfo>");
    qRegisterMetaType<TablePartitioningInfo>("TablePartitioningInfo");
    qRegisterMetaType<TableExternalInfo>("TableExternalInfo");
    qRegisterMetaType<DbTreeModel::DbTreeNodeType>("DbTreeModel::DbTreeNodeType");
}
