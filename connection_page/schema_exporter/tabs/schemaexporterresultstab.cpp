#include "schemaexporterresultstab.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include <QtGui>

SchemaExporterResultsTab::SchemaExporterResultsTab(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparerResultsTab(uiManager, parent)
{
}



void SchemaExporterResultsTab::objectExportStarted(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName)
{
    QList<QStandardItem*> rowItems;
    rowItems.append(new QStandardItem(IconUtil::getIcon(DbUtil::getDbObjectIconNameByParentNodeType(parentNodeType)), objectName));
    rowItems.append(new QStandardItem(DbUtil::getDbObjectTypeNameByNodeType(parentNodeType)));
    rowItems.append(new QStandardItem(tr("Exporting...")));
    tableModel->appendRow(rowItems);

    changeListTable->scrollToBottom();
}

void SchemaExporterResultsTab::objectExportCompleted(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName)
{
    Q_UNUSED(parentNodeType);
    Q_UNUSED(objectName);

    int rowCount = tableModel->rowCount();
    Q_ASSERT(rowCount>0);

    QStandardItem* lastStatusItem = tableModel->item(rowCount-1, 2);
    Q_ASSERT(lastStatusItem);

    lastStatusItem->setText(tr("Exported"));
}

void SchemaExporterResultsTab::setCurrentSchema(const QString &currentSchemaName)
{
    changeListTable->setObjectListMode(-1, 0, -1, 1, currentSchemaName);
}
