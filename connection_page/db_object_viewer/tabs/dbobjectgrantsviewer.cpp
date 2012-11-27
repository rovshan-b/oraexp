#include "dbobjectgrantsviewer.h"

DbObjectGrantsViewer::DbObjectGrantsViewer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab("get_table_grants_for_detailed_view", uiManager, parent)
{
    setIconColumn("GRANTOR", "GRANTOR_ICON_COLUMN");
    setIconColumn("GRANTEE", "GRANTEE_ICON_COLUMN");
    setObjectListMode(1, 1, -1, QString(), "SCHEMA");
}
