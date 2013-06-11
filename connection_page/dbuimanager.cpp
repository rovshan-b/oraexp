#include "dbuimanager.h"
#include "../navtree/dbtreeitem.h"
#include "../navtree/nodeaction.h"
#include "beans/dbitemaction.h"
#include "beans/dbitemdynamicaction.h"
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
#include "session_browser/sessionbrowser.h"
#include "dialogs/dmlgeneratordialog.h"
#include "dialogs/tableddlgeneratordialog.h"
#include "dialogs/tablenamefinderdialog.h"
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
    createEditor(DbObjectCreator::CreateNew);
}

//called by top level menu items
void DbUiManager::createCreator(DbItemAction *action)
{
    QString actionSchema=action->getSchemaName();
    createEditor(actionSchema.isEmpty() ? db->getSchemaName() : actionSchema,
                 action->getObjectName(),
                 action->getItemType(),
                 DbObjectCreator::CreateNew,
                 action->properties);
}

void DbUiManager::createCreator(const QString &schemaName,
                               const QString &objectName,
                               const DbTreeModel::DbTreeNodeType itemType)
{
    createEditor(schemaName, objectName, itemType, DbObjectCreator::CreateNew);
}

void DbUiManager::createCreator(const QString &schemaName, const QString &objectName, const QString &itemTypeName)
{
    createCreator(schemaName, objectName, DbTreeModel::getDbTreeNodeType(itemTypeName));
}

void DbUiManager::createEditor(DbObjectCreator::CreatorMode creatorMode)
{
    DbItemAction *action=getSenderAction();
    QString schemaName=action->getSchemaName();
    if(schemaName.isEmpty()){
        schemaName=db->getSchemaName();
    }
    createEditor(schemaName, action->getObjectName(), action->getItemType(), creatorMode, action->properties);
}

void DbUiManager::createEditor(const QString &schemaName,
                               const QString &objectName,
                               const DbTreeModel::DbTreeNodeType itemType,
                               DbObjectCreator::CreatorMode creatorMode,
                               QHash<QString,QString> properties)
{
    QString objectTypeName = DbUtil::getDbObjectTypeNameByNodeType(itemType).toLower();
    QString tabId = createTabId("editor", objectTypeName, schemaName, objectName);
    ConnectionPageTab * existingTab = (creatorMode == DbObjectCreator::EditExisting) ? cnPage->findTabById(tabId) : 0;
    if(existingTab){
        setProperties(existingTab);
        existingTab->setPropertyValue(ConnectionPageTab::TAB_NAME_KEY, properties.value(ConnectionPageTab::TAB_NAME_KEY));
        existingTab->setPropertyValue(ConnectionPageTab::CHILD_OBJECT_NAME_KEY, properties.value(ConnectionPageTab::CHILD_OBJECT_NAME_KEY));
        existingTab->highlightChildObject();
        cnPage->setCurrentTab(existingTab);
        return;
    }

    ConnectionPageTab *editor = EditorCreatorUtil::createEditor(schemaName,
                                                      objectName,
                                                      itemType,
                                                      this,
                                                      creatorMode);
    editor->setTabId(tabId);
    editor->setProperties(properties);

    QString iconName = DbUtil::getDbObjectIconNameByParentNodeType(itemType);
    if(creatorMode == DbObjectCreator::EditExisting){
        iconName.append("_alter");
    }else{
        iconName.append("_add");
    }
    cnPage->addTab(editor, IconUtil::getIcon(iconName), (creatorMode == DbObjectCreator::EditExisting) ? objectName : QString("Create %1").arg(objectTypeName));
}

//this function is called by UiManagerInvoker through context menus
void DbUiManager::createEditor(const QString &schemaName, const QString &objectName, const QString &objectTypeName,
                               const QString &tabName, const QString &childObjectName)
{
    createEditor(schemaName, objectName, DbTreeModel::getDbTreeNodeType(objectTypeName),
                 tabName, childObjectName);
}

void DbUiManager::createLikeEditor(const QString &schemaName, const QString &objectName, int itemType)
{
    createEditor(schemaName,
                 objectName,
                 (DbTreeModel::DbTreeNodeType)itemType,
                 DbObjectCreator::CreateLike);
}

void DbUiManager::createEditor(const QString &schemaName, const QString &objectName, int itemType,
                               const QString &tabName, const QString &childObjectName)
{
    QHash<QString,QString> properties;
    if(!tabName.isEmpty()){
        properties[ConnectionPageTab::TAB_NAME_KEY] = tabName;

        if(!childObjectName.isEmpty()){
            properties[ConnectionPageTab::CHILD_OBJECT_NAME_KEY] = childObjectName;
        }
    }

    if(((DbTreeModel::DbTreeNodeType)itemType) == DbTreeModel::Table && objectName.isEmpty()){ //indexes and constraints can call without table name set
        TableNameFinderDialog *tableNameFinder = new TableNameFinderDialog(schemaName, tabName, childObjectName, this);
        addWindow(tableNameFinder, IconUtil::getIcon("table"), tr("Loading..."));
    }else{
        createEditor(schemaName, objectName, (DbTreeModel::DbTreeNodeType)itemType, DbObjectCreator::EditExisting, properties);
    }
}

void DbUiManager::createViewer()
{
    DbItemAction *action=getSenderAction();
    createViewer(action->getSchemaName(), action->getObjectName(), action->getItemType());
}

void DbUiManager::createViewer(const QString &schemaName, const QString &objectName, int itemType /*DbTreeModel::DbTreeNodeType*/)
{
    QString objectTypeName = DbUtil::getDbObjectTypeNameByNodeType((DbTreeModel::DbTreeNodeType)itemType).toLower();
    QString tabId = createTabId("viewer", objectTypeName, schemaName, objectName);
    ConnectionPageTab * existingTab = cnPage->findTabById(tabId);
    if(existingTab){
        cnPage->setCurrentTab(existingTab);
        return;
    }

    ConnectionPageTab *viewer = EditorCreatorUtil::createViewer(schemaName,
                                                                objectName,
                                                                (DbTreeModel::DbTreeNodeType)itemType,
                                                                this);
    viewer->setTabId(tabId);
    QString iconName = DbUtil::getDbObjectIconNameByParentNodeType((DbTreeModel::DbTreeNodeType)itemType);
    cnPage->addTab(viewer, IconUtil::getIcon(iconName), objectName);
}

QString DbUiManager::createTabId(const QString &prefix, const QString &objectTypeName, const QString &schemaName, const QString &objectName) const
{
    QString result = QString("%1.%2.%3.%4").arg(prefix, objectTypeName, schemaName, objectName);
    result.remove(" SPEC").remove(" BODY"); //use same editor for package/type spec and body
    return result;
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

Worksheet *DbUiManager::getWorksheet()
{
    ConnectionPageTab *currentTab = cnPage->currentConnectionPageTab();
    if(currentTab == 0){
        return addWorksheet();
    }

    if(currentTab != 0 && currentTab->metaObject()->className() == Worksheet::staticMetaObject.className()){
        return static_cast<Worksheet*>(currentTab);
    }else{
        return addWorksheet();
    }
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

void DbUiManager::addDataComparer(const QString &schemaName, const QString &objectName)
{
    DataComparer *dataComparer=new DataComparer(this);
    setProperties(dataComparer, schemaName, objectName);
    cnPage->addTab(dataComparer, IconUtil::getIcon("compare_data"), tr("Compare data"));
}

void DbUiManager::addDataCopier(const QString &schemaName, const QString &objectName)
{
    DataCopier *dataCopier=new DataCopier(this);
    setProperties(dataCopier, schemaName, objectName);
    cnPage->addTab(dataCopier, IconUtil::getIcon("copy_data"), tr("Copy data"));
}

void DbUiManager::addDataExporter(const QString &schemaName, const QString &objectName)
{
    DataExporter *dataExporter=new DataExporter(this);
    setProperties(dataExporter, schemaName, objectName);
    cnPage->addTab(dataExporter, IconUtil::getIcon("export"), tr("Export data"));
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
    DataImporter *dataImporter=new DataImporter(schemaName, tableName, this);
    addWindow(dataImporter, IconUtil::getIcon("import_data"), tr("Import data"));
}

void DbUiManager::setProperties(ConnectionPageTab *tab, const QString &schemaName, const QString &objectName)
{
    tab->setPropertyValue("schemaName", schemaName);
    tab->setPropertyValue("objectName", objectName);
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

void DbUiManager::addDmlGenerator(const QString &schemaName, const QString &tableName, int dmlType /*OraExp::DmlType*/)
{
    DmlGeneratorDialog *dmlGenerator = new DmlGeneratorDialog(this, schemaName, tableName, (OraExp::DmlType)dmlType);
    addWindow(dmlGenerator, IconUtil::getIcon("query"), tr("Generate DML"));
}

void DbUiManager::addDdlGenerator(const QString &schemaName, const QString &tableName)
{
    TableDdlGeneratorDialog *ddlGenerator = new TableDdlGeneratorDialog(this, schemaName, tableName);
    addWindow(ddlGenerator, IconUtil::getIcon("ddl"), tr("Generate DDL"));
}

void DbUiManager::addSessionBrowser()
{
    SessionBrowser *sessionBrowser = new SessionBrowser(this);
    cnPage->addTab(sessionBrowser, IconUtil::getIcon("session_browser"), tr("Session browser"));
}

void DbUiManager::addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title)
{
    cnPage->addWindow(window, icon, title);
}

void DbUiManager::handleDynamicAction()
{
    DbItemDynamicAction *action = static_cast<DbItemDynamicAction*>(sender());
    Q_ASSERT(action);
    action->execute(this);
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
                //cnPage->setCurrentTab(worksheet);
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
