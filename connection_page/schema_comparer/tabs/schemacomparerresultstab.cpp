#include "schemacomparerresultstab.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "widgets/datatable.h"
#include <QtGui>

SchemaComparerResultsTab::SchemaComparerResultsTab(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparerResultsTab(uiManager, parent)
{

}

void SchemaComparerResultsTab::setCurrentSchema(const QString &currentSchemaName)
{
    changeListTable->setObjectListMode(-1, 0, -1, 1, currentSchemaName);
}

void SchemaComparerResultsTab::changedObjectDetected(const QString &objectName, DbTreeModel::DbTreeNodeType objectType, const QString &compareResult)
{
    QList<QStandardItem*> rowItems;
    rowItems.append(new QStandardItem(IconUtil::getIcon(DbUtil::getDbObjectIconNameByParentNodeType(objectType)), objectName));
    rowItems.append(new QStandardItem(DbUtil::getDbObjectTypeNameByNodeType(objectType)));
    rowItems.append(new QStandardItem(compareResult=="new" ?  tr("New") : tr("Changed")));
    tableModel->appendRow(rowItems);

    changeListTable->scrollToBottom();
}
