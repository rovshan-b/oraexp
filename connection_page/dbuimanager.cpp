#include "dbuimanager.h"
#include "../navtree/dbtreeitem.h"
#include "../navtree/nodeaction.h"
#include "beans/dbitemaction.h"
#include "../navtree/dbtreeitem.h"
#include "connectionpage.h"
#include "connectivity/dbconnection.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "util/dialoghelper.h"
#include "util/filesystemutil.h"
#include "worksheet/worksheet.h"
#include "schema_comparer/schemacomparer.h"
#include "schema_exporter/schemaexporter.h"
#include "data_comparer/datacomparer.h"
#include "data_copier/datacopier.h"
#include "data_exporter/dataexporter.h"
#include "data_importer/dataimporter.h"
#include "editorcreatorutil.h"
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

void DbUiManager::createCreator()
{
    createEditor(false);
}

//called by top level menu items
void DbUiManager::createCreator(DbItemAction *action)
{
    QString actionSchema=action->getSchemaName();
    createEditor(actionSchema.isEmpty() ? db->getSchemaName() : actionSchema,
                 action->getObjectName(),
                 action->getItemType(),
                 false,
                 action->properties);
}

void DbUiManager::createCreator(const QString &schemaName,
                               const QString &objectName,
                               const DbTreeModel::DbTreeNodeType itemType)
{
    createEditor(schemaName, objectName, itemType, false);
}

void DbUiManager::createEditor(bool editMode)
{
    DbItemAction *action=getSenderAction();
    QString schemaName=action->getSchemaName();
    if(schemaName.isEmpty()){
        schemaName=db->getSchemaName();
    }
    createEditor(schemaName, action->getObjectName(), action->getItemType(), editMode, action->properties);
}

void DbUiManager::createEditor(const QString &schemaName,
                               const QString &objectName,
                               const DbTreeModel::DbTreeNodeType itemType,
                               bool editMode,
                               QHash<QString,QString> properties)
{
    QString objectTypeName = DbUtil::getDbObjectTypeNameByNodeType(itemType).toLower();
    QString tabId = QString("editor.%1.%2.%3").arg(objectTypeName, schemaName, objectName);
    ConnectionPageTab * existingTab = cnPage->findTabById(tabId);
    if(existingTab){
        cnPage->setCurrentTab(existingTab);
        return;
    }

    ConnectionPageTab *editor = EditorCreatorUtil::createEditor(schemaName,
                                                      objectName,
                                                      itemType,
                                                      this);
    editor->setTabId(tabId);
    editor->setProperties(properties);

    QString iconName = DbUtil::getDbObjectIconNameByParentNodeType(itemType);
    if(editMode){
        iconName.append("_alter");
    }else{
        iconName.append("_add");
    }
    cnPage->addTab(editor, IconUtil::getIcon(iconName), editMode ? objectName : QString("Create %1").arg(objectTypeName));
}

void DbUiManager::createViewer()
{
    DbItemAction *action=getSenderAction();
    createViewer(action->getSchemaName(), action->getObjectName(), action->getItemType());
}

void DbUiManager::createViewer(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType)
{
    QString objectTypeName = DbUtil::getDbObjectTypeNameByNodeType(itemType).toLower();
    QString tabId = QString("viewer.%1.%2.%3").arg(objectTypeName, schemaName, objectName);
    ConnectionPageTab * existingTab = cnPage->findTabById(tabId);
    if(existingTab){
        cnPage->setCurrentTab(existingTab);
        return;
    }

    ConnectionPageTab *viewer = EditorCreatorUtil::createViewer(schemaName,
                                                                objectName,
                                                                itemType,
                                                                this);
    viewer->setTabId(tabId);
    QString iconName = DbUtil::getDbObjectIconNameByParentNodeType(itemType);
    cnPage->addTab(viewer, IconUtil::getIcon(iconName), objectName);
}

Worksheet *DbUiManager::addWorksheet(const QString &contents)
{
    Worksheet *worksheet=new Worksheet(this);
    cnPage->addTab(worksheet, IconUtil::getIcon("worksheet"), tr("Worksheet"));
    if(!contents.isEmpty()){
        worksheet->setContents(contents);
    }

    return worksheet;
}

DbItemAction *DbUiManager::getSenderAction() const
{
    DbItemAction* itemAction=dynamic_cast<DbItemAction*>(sender());
    Q_ASSERT(itemAction);

    return itemAction;
}

void DbUiManager::addSchemaComparer()
{
    SchemaComparer *schemaComparer=new SchemaComparer(this);
    setProperties(schemaComparer);
    cnPage->addTab(schemaComparer, IconUtil::getIcon("compare_schemas"), tr("Compare schemas"));
}

void DbUiManager::addSchemaExporter()
{
    SchemaExporter *schemaExporter=new SchemaExporter(this);
    setProperties(schemaExporter);
    cnPage->addTab(schemaExporter, IconUtil::getIcon("export_schema"), tr("Export schema"));
}

void DbUiManager::addDataComparer()
{
    DataComparer *dataComparer=new DataComparer(this);
    setProperties(dataComparer);
    cnPage->addTab(dataComparer, IconUtil::getIcon("compare_data"), tr("Compare data"));
}

void DbUiManager::addDataCopier()
{
    DataCopier *dataCopier=new DataCopier(this);
    setProperties(dataCopier);
    cnPage->addTab(dataCopier, IconUtil::getIcon("copy_data"), tr("Copy data"));
}

void DbUiManager::addDataExporter()
{
    DataExporter *dataExporter=new DataExporter(this);
    setProperties(dataExporter);
    cnPage->addTab(dataExporter, IconUtil::getIcon("export"), tr("Export data"));
}

void DbUiManager::setProperties(ConnectionPageTab *tab)
{
    if(sender()){
        DbItemAction *action=getSenderAction();
        QHash<QString,QString> properties;
        QHashIterator<QString,QString> i(action->properties);
         while (i.hasNext()) {
             i.next();
             properties[i.key()] = i.value();
         }
         properties["schemaName"] = action->getSchemaName();
         properties["objectName"] = action->getObjectName();

         tab->setProperties(properties);
    }
}

void DbUiManager::addDataImporter()
{
    if(sender()){
        DbItemAction *action=getSenderAction();
        addDataImporter(action->getSchemaName(), action->getObjectName());
    }else{
        addDataImporter(db->getSchemaName(), "");
    }
}

void DbUiManager::addDataImporter(const QString &schemaName, const QString &tableName)
{
    DataImporter *dataImporter=new DataImporter(schemaName, tableName);
    cnPage->addWindow(dataImporter, IconUtil::getIcon("import_data"), tr("Import data"));
}

void DbUiManager::handleDynamicAction()
{
}

void DbUiManager::openFile()
{
    QString fileName = DialogHelper::showFileOpenDialog(cnPage->window(),
                                     tr("Text files (*.sql *.pkc *.pks *.pkb *.prc *.fnc *.trg *.txt);;All files (*.*)"));

    if(fileName.isEmpty()){
        return;
    }

    QList<ConnectionPageTab *> worksheets = cnPage->getTabsByType(Worksheet::staticMetaObject.className());
    foreach(ConnectionPageTab *worksheet, worksheets){
        if(worksheet->getCurrentFileName() == fileName){
            cnPage->setCurrentTab(worksheet);
            return;
        }
    }

    QString errorMessage;
    QString contents;
    contents = FileSystemUtil::readAsString(fileName, &errorMessage);
    if(contents.isEmpty() && !errorMessage.isEmpty()){
        QMessageBox::critical(cnPage->window(),
                              tr("Failed to open file"),
                              errorMessage);
    }else{
        Worksheet *worksheet = 0;

        ConnectionPageTab *currentTab = cnPage->currentConnectionPageTab();
        if(currentTab != 0 && currentTab->metaObject()->className() == Worksheet::staticMetaObject.className()){
            Worksheet *currentWorksheet = static_cast<Worksheet*>(currentTab);
            if(!currentWorksheet->isModified() &&
                    currentWorksheet->getCurrentFileName().isEmpty() &&
                    currentWorksheet->getContents().isEmpty()){

                worksheet = currentWorksheet;

                worksheet->setContents(contents);
                cnPage->setCurrentTab(worksheet);
            }
        }

        if(worksheet == 0){
            worksheet = addWorksheet(contents);
        }
        worksheet->setCurrentFileName(fileName);
    }
}

void DbUiManager::closeTab(QWidget *widget)
{
    cnPage->closeTab(widget);
}

QList<ConnectionPageTab *> DbUiManager::getTabsByConnection(DbConnection *db, const QString &className, int limit)
{
    return cnPage->getTabsByConnection(db, className, limit);
}
