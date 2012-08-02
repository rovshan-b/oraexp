#include "schemacomparerresultstab.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include <QtGui>

SchemaComparerResultsTab::SchemaComparerResultsTab(QWidget *parent) :
    DbObjectComparerResultsTab(parent)
{

}

void SchemaComparerResultsTab::changedObjectDetected(const QString &objectName, DbTreeModel::DbTreeNodeType objectType, const QString &compareResult)
{
    QList<QStandardItem*> rowItems;
    rowItems.append(new QStandardItem(IconUtil::getIcon(DbUtil::getDbObjectIconNameByParentNodeType(objectType)), objectName));
    rowItems.append(new QStandardItem(DbUtil::getDbObjectTypeNameByParentNodeType(objectType)));
    rowItems.append(new QStandardItem(compareResult=="new" ?  tr("New") : tr("Changed")));
    tableModel->appendRow(rowItems);
}
