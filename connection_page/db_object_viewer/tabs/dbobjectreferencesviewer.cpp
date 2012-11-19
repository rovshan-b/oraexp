#include "dbobjectreferencesviewer.h"
#include "util/dbutil.h"

DbObjectReferencesViewer::DbObjectReferencesViewer(bool references, QWidget *parent) :
    DbObjectViewerGenericTab(references ? "get_object_references_for_detailed_view" : "get_object_dependecies_for_detailed_view",
    parent)
{
    setIconColumn("NAME", "ICON_NAME");
}

QList<Param *> DbObjectReferencesViewer::getQueryParams()
{
    return DbObjectViewerGenericTab::getQueryParams() <<
           new Param(":object_type", DbUtil::getDbObjectTypeNameByNodeType(itemType));
}
