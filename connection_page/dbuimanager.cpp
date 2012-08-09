#include "dbuimanager.h"
#include "../navtree/dbtreeitem.h"
#include "../navtree/nodeaction.h"
#include "../navtree/dbtreemodel.h"
#include "../navtree/dbtreeitem.h"
#include "connectionpage.h"
#include "connectivity/dbconnection.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "worksheet/worksheet.h"
#include "table_info/tableinfoviewer.h"
#include "table_creator/tablecreator.h"
#include "schema_comparer/schemacomparer.h"
#include "data_comparer/datacomparer.h"
#include "code_creator/codecreator.h"
#include <QtGui>

DbUiManager::DbUiManager(DbConnection *db, QObject *parent) :
    QObject(parent)
{
    this->db=db;

    cnPage=(ConnectionPage*)parent;
}

void DbUiManager::refreshTreeNodeChildren()
{
    const QModelIndex index=((NodeAction*)sender())->getModelIndex();
    ((DbTreeModel*)index.model())->refreshChildren(index);
}

void DbUiManager::viewTableDetails()
{
    const QModelIndex index=((NodeAction*)sender())->getModelIndex();
    DbTreeItem *node=(DbTreeItem*)index.internalPointer();
    viewTableDetails(node->schemaName(), node->itemName());
}

QString DbUiManager::getCorrectSchemaNameForCurrentContext()
{
    NodeAction* nodeAction=dynamic_cast<NodeAction*>(sender());
    if(nodeAction){
        const QModelIndex index=((NodeAction*)sender())->getModelIndex();
        DbTreeItem *node=(DbTreeItem*)index.internalPointer();
        return node->schemaName();
    }else{
        return db->getSchemaName();
    }
}

void DbUiManager::showTableCreator()
{
    showTableCreator(getCorrectSchemaNameForCurrentContext(), "");
}

void DbUiManager::showViewCreator()
{
    showViewCreator(getCorrectSchemaNameForCurrentContext(), "");
}

void DbUiManager::addWorksheet(const QString &contents)
{
    Worksheet *worksheet=new Worksheet(this);
    cnPage->addTab(worksheet, IconUtil::getIcon("worksheet"), tr("Worksheet"));
    if(!contents.isEmpty()){
        worksheet->setContents(contents);
    }
}

void DbUiManager::viewTableDetails(const QString &schemaName, const QString &tableName)
{
    TableInfoViewer *tableInfoViewer=new TableInfoViewer(schemaName, tableName, this);
    cnPage->addTab(tableInfoViewer, IconUtil::getIcon("table"), tableName);
}

void DbUiManager::showTableCreator(const QString &schemaName, const QString &tableName)
{
    TableCreator *tableCreator=new TableCreator(schemaName, tableName, this);

    QString iconName=(tableName.isEmpty() ? "table_add" : "table_alter");
    QString tabTitle=(tableName.isEmpty() ? tr("Create table") : tr("Alter %1").arg(tableName));

    cnPage->addTab(tableCreator, IconUtil::getIcon(iconName), tabTitle);
}

void DbUiManager::showViewCreator(const QString &schemaName, const QString &viewName)
{
    CodeCreator *codeCreator=new CodeCreator(schemaName, viewName,
                                             DbTreeModel::View,
                                             this);

    QString iconName=(viewName.isEmpty() ? "view_add" : "view_alter");
    QString tabTitle=(viewName.isEmpty() ? tr("Create view") : tr("Alter %1").arg(viewName));

    cnPage->addTab(codeCreator, IconUtil::getIcon(iconName), tabTitle);
}

void DbUiManager::alterTable()
{
    const QModelIndex index=((NodeAction*)sender())->getModelIndex();
    DbTreeItem *node=(DbTreeItem*)index.internalPointer();
    showTableCreator(node->schemaName(), node->itemName());
}

void DbUiManager::alterView()
{
    const QModelIndex index=((NodeAction*)sender())->getModelIndex();
    DbTreeItem *node=(DbTreeItem*)index.internalPointer();
    showViewCreator(node->schemaName(), node->itemName());
}

void DbUiManager::addSchemaComparer()
{
    SchemaComparer *schemaComparer=new SchemaComparer(this);
    cnPage->addTab(schemaComparer, IconUtil::getIcon("compare_schemas"), tr("Compare schemas"));
}

void DbUiManager::addDataComparer()
{
    DataComparer *dataComparer=new DataComparer(this);
    cnPage->addTab(dataComparer, IconUtil::getIcon("compare_data"), tr("Compare data"));
}

void DbUiManager::closeTab(QWidget *widget)
{
    cnPage->closeTab(widget);
}
