#include "contextmenuutil.h"
#include "viewcontextmenuutil.h"
#include "packagecontextmenuutil.h"
#include "procedurecontextmenuutil.h"
#include "functioncontextmenuutil.h"
#include "triggercontextmenuutil.h"
#include "indexcontextmenuutil.h"
#include "sequencecontextmenuutil.h"
#include "typecontextmenuutil.h"
#include "synonymcontextmenuutil.h"
#include "dblinkcontextmenuutil.h"
#include "usercontextmenuutil.h"

#include "action_creators/dbitemdynamicactioncreator.h"
#include "action_creators/dynamicactioncreator.h"
#include "util/filesystemutil.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "navtree/dbtreemodel.h"
#include <QFile>
#include <QDomDocument>
#include <QDebug>

ContextMenuUtil::ContextMenuUtil()
{
}

QList<QAction *> ContextMenuUtil::getActionsForObject(const QString &schemaName,
                                                      const QString &objectName,
                                                      const QString &parentObjectName,
                                                      const DbTreeModel::DbTreeNodeType itemType,
                                                      DbUiManager *uiManager)
{
    QList<QAction*> results;

    switch(itemType){
    case DbTreeModel::Views:
    case DbTreeModel::View:
        results = ViewContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
    case DbTreeModel::PackageSpec:
    case DbTreeModel::PackageBody:
        results = PackageContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        results = ProcedureContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Functions:
    case DbTreeModel::Function:
        results = FunctionContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::SchemaTriggers:
    case DbTreeModel::Triggers:
    case DbTreeModel::Trigger:
        results = TriggerContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Index:
        results = IndexContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Sequences:
    case DbTreeModel::Sequence:
        results = SequenceContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpec:
    case DbTreeModel::TypeBody:
        results = TypeContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Synonyms:
    case DbTreeModel::Synonym:
        results = SynonymContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::DatabaseLinks:
    //case DbTreeModel::PublicDatabaseLinks:
    case DbTreeModel::DatabaseLink:
        results = DbLinkContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::AllSchemas:
    case DbTreeModel::Schema:
        results = UserContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    default:
        break;
    }

    QList<QAction *> configuredActions = ContextMenuUtil::getActionsForObjectFromConfiguration(schemaName, objectName, parentObjectName,
                                                          itemType, uiManager);
    if(configuredActions.size() > 0){
        results.append(WidgetHelper::createSeparatorAction());
        results.append(configuredActions);
    }

    return results;
}

QList<QAction *> ContextMenuUtil::getActionsFromFile(const QString &baseName,
                                                     DbUiManager *uiManager,
                                                     QObject *resultListener,
                                                     const QHash<QString, QString> &properties)
{
    QDomElement docElem;
    if(!ContextMenuUtil::readFromFile(baseName, docElem)){
        return QList<QAction*>();
    }

    DynamicActionCreator actionCreator(uiManager, properties);

    return ContextMenuUtil::actionsFromElement(docElem, &actionCreator, 0, resultListener);
}

bool ContextMenuUtil::readFromFile(const QString &baseName, QDomElement &rootElement)
{
    QString fileName = QString(":/context_menus/%1.xml").arg(baseName);

    if(!QFile::exists(fileName)){
        return false;
    }

    QString errorMessage;
    QString fileContents = FileSystemUtil::readAsString(fileName, &errorMessage);

    Q_ASSERT(!fileContents.isEmpty() && errorMessage.isEmpty());

    QDomDocument doc;
    if (!doc.setContent(fileContents)) {
        Q_ASSERT(false);
        return false;
    }

    rootElement = doc.documentElement();

    return true;
}

QList<QAction *> ContextMenuUtil::getActionsForObjectFromConfiguration(const QString &schemaName,
                                                                       const QString &objectName,
                                                                       const QString &parentObjectName,
                                                                       const DbTreeModel::DbTreeNodeType itemType,
                                                                       DbUiManager *uiManager)
{
    QString nodeName = DbTreeModel::getDbTreeNodeName(itemType);

    QDomElement docElem;
    if(!ContextMenuUtil::readFromFile(nodeName, docElem)){
        return QList<QAction*>();
    }

    DbItemDynamicActionCreator actionCreator(schemaName, objectName, parentObjectName, itemType, uiManager);

    return ContextMenuUtil::actionsFromElement(docElem, &actionCreator, 0, 0);
}

QList<QAction *> ContextMenuUtil::actionsFromElement(const QDomElement &element, ContextMenuActionCreator *actionCreator, QObject *parent, QObject *resultListener)
{
    QList<QAction *> results;

    QDomNode n = element.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull() && e.tagName() == "item") {
            results.append(ContextMenuUtil::actionFromElement(e, actionCreator, parent, resultListener));
        }
        n = n.nextSibling();
    }

    return results;
}

QAction *ContextMenuUtil::actionFromElement(const QDomElement &e, ContextMenuActionCreator *actionCreator, QObject *parent, QObject *resultListener)
{
    QString caption = e.attribute("caption");

    if(e.hasChildNodes()){
        QAction *action = new QAction(caption, parent);
        QMenu *actionMenu = new QMenu(caption);
        actionMenu->addActions(ContextMenuUtil::actionsFromElement(e, actionCreator, actionMenu, resultListener));
        action->setMenu(actionMenu);
        return action;
    }else{

        if(caption == "-"){
            return WidgetHelper::createSeparatorAction(parent);
        }else{
            DynamicAction *action = actionCreator->create(IconUtil::getIcon(e.attribute("icon")), caption, parent);
            action->resultListener = resultListener;

            QDomNamedNodeMap attributes = e.attributes();
            for(int i=0; i<attributes.size(); ++i){
                QDomAttr attribute = attributes.item(i).toAttr();
                action->properties[QString("attribute.%1").arg(attribute.name())] = attribute.value();
            }

            return action;
        }
    }

}
