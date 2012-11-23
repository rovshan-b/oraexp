#include "dbobjectgrantsviewer.h"

DbObjectGrantsViewer::DbObjectGrantsViewer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab("get_table_grants_for_detailed_view", uiManager, parent)
{
    setIconColumn("GRANTEE", "ICON_COLUMN");
    setObjectListMode(1, 1, -1, QString(), "SCHEMA");
}
