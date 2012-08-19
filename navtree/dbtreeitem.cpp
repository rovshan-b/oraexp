#include "dbtreeitem.h"
#include <iostream>
#include <QStringList>
#include <QIcon>
#include <QList>
#include <QAction>
#include <memory>
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include "connectivity/statement.h"
#include "util/queryutil.h"
#include "connectivity/ociexception.h"
#include "util/strutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"
#include "nodeaction.h"
#include "context_menu/contextmenuutil.h"
#include <QMessageBox>

using namespace std;

/*DbTreeItem::DbTreeItem(const QList<QVariant> &data, DbTreeItem *parent)
{
    itemData=data;
    parentItem=parent;
}*/

DbTreeItem::DbTreeItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        m_parentItem(parent),
        m_childrenPopulated(false),
        m_itemText(itemText),
        m_itemName(itemName),
        m_tooltip(itemText),
        m_isInactive(false),
        m_checkState(Qt::Checked),
        m_isTopLevel(true)
{
    this->m_model=model;
}

DbTreeItem::~DbTreeItem()
{
    qDeleteAll(m_childItems);

    //cout << "destructor called for DbTreeItem" << endl;
}

void DbTreeItem::appendChild(DbTreeItem *child)
{
    if(child->schemaName().isEmpty()){
        child->setSchemaName(this->schemaName());
    }

    child->m_parentItem=this;
    m_childItems.append(child);

    if(!m_isTopLevel){
        child->setIsTopLevel(false);
    }
}

DbTreeItem *DbTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int DbTreeItem::childCount() const
{
    return m_childItems.count();
}

int DbTreeItem::columnCount() const
{
    return 1;
}

QVariant DbTreeItem::data(int /*column*/, int role) const
{
    if(role==Qt::DisplayRole){
        return m_itemText;
    }else if(role==Qt::ToolTipRole){
        return m_tooltip;
    }else if(role==Qt::DecorationRole){
        //return IconUtil::getIcon(iconName(), isInactive);
        return IconUtil::getIcon(iconName());
    }else if(role==Qt::CheckStateRole && getModel()->checkboxesEnabled() && checkBehavior()!=NotCheckable){
        return m_checkState;
    }else{
        return QVariant();
    }
}

DbTreeItem *DbTreeItem::parent() const
{
    return m_parentItem;
}

int DbTreeItem::row() const
{
    if(this==0){
        cout << "called row with this=" << this << endl;
    }
    if(m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<DbTreeItem*>(this));
    }

    return 0;
}

QString DbTreeItem::iconName() const
{
    return "";
}

bool DbTreeItem::areChildrenPopulated() const
{
    return m_childrenPopulated;
}

void DbTreeItem::setChildrenPopulated(bool populated)
{
    m_childrenPopulated=populated;
}

QList<DbTreeItem*> DbTreeItem::populateChildren()
{
    //to be reimplemented in child classes

    return QList<DbTreeItem*>();
}

bool DbTreeItem::hasChildren() const
{
    return true;
}

DbTreeModel *DbTreeItem::getModel() const
{
    return m_model;
}

QString DbTreeItem::itemName() const
{
    return m_itemName;
}

QList<DbTreeItem*> DbTreeItem::createNodeForEachRecord(const QString &select, Param *param1)
{
    QList<Param*> parameters;
    parameters.append(param1);
    return createNodeForEachRecord(select, parameters);
}
QList<DbTreeItem*> DbTreeItem::createNodeForEachRecord(const QString &select, Param *param1, Param *param2)
{
    QList<Param*> parameters;
    parameters.append(param1);
    parameters.append(param2);
    return createNodeForEachRecord(select, parameters);
}
QList<DbTreeItem*> DbTreeItem::createNodeForEachRecord(const QString &select, Param *param1, Param *param2, Param *param3)
{
    QList<Param*> parameters;
    parameters.append(param1);
    parameters.append(param2);
    parameters.append(param3);
    return createNodeForEachRecord(select, parameters);
}

QList<DbTreeItem*> DbTreeItem::createNodeForEachRecord(const QString &select, QList<Param*> parameters)
{
    QList<DbTreeItem*> results;

    try{
        QueryResult result=getModel()->getDb()->executeQuery(select, parameters);
        Resultset * rs=result.statement->rsAt(0);
        auto_ptr<Statement> autoStmt(result.statement); //for automatically releasing pointer

        DbTreeItem* item;

        rs->beginFetchRows();
        while(rs->moveNext()){
            item=createNodeFromRecord(rs);

            //set check state
            if(getModel()->checkboxesEnabled() &&
                    item->m_parentItem!=0 &&
                    item->m_parentItem->checkBehavior()==CheckableForTogglingChildren &&
                    item->checkBehavior()==Checkable){
                 item->setCheckState(item->m_parentItem->checkState()==Qt::Unchecked ? Qt::Unchecked : Qt::Checked);
            }

            results.append(item);
        }
        rs->endFetchRows();

    }catch(OciException &ex){

        qDeleteAll(results);
        results.clear();

        cerr << ex.what() << endl;

        throw ex;
    }

    return results;
}

DbTreeItem *DbTreeItem::createNodeFromRecord(Resultset *)
{
    return NULL;
}


QString DbTreeItem::tooltip() const
{
    return m_tooltip;
}

void DbTreeItem::setTooltip(const QString &tooltip)
{
    this->m_tooltip=tooltip;
}

QString DbTreeItem::schemaName() const
{
    return this->m_dbSchemaName;
}

void DbTreeItem::setSchemaName(const QString &schemaName)
{
    this->m_dbSchemaName=schemaName;
}

bool DbTreeItem::isInCurrentUsersSchema() const
{
    return (getModel()->getDb()->getSchemaName()==schemaName());
}

QString DbTreeItem::itemText() const
{
    return this->m_itemText;
}

void DbTreeItem::setItemText(const QString &itemText)
{
    this->m_itemText=itemText;
}

QString DbTreeItem::getQuery(const QString &queryName) const
{
    return QueryUtil::getQuery(queryName, m_model->getDb());
}

void DbTreeItem::setInactive(bool inactive)
{
    this->m_isInactive=inactive;
}

bool DbTreeItem::displayChildCount() const
{
    return true;
}

void DbTreeItem::clearChildItems(bool resetPopulatedFlag)
{
    qDeleteAll(m_childItems);
    m_childItems.clear();

    if(resetPopulatedFlag){
        setChildrenPopulated(false);
    }
}

QList<QAction*> DbTreeItem::getContextMenuItems(const QModelIndex &index) const
{
    QList<QAction*> actions;

    if(hasChildren()){
        NodeAction *refreshAction=new NodeAction(index, IconUtil::getIcon("refresh"), QObject::tr("Refresh children"));
        QObject::connect(refreshAction, SIGNAL(triggered()), this->getModel()->getUiManager(), SLOT(refreshTreeNodeChildren()));
        actions.append(refreshAction);
    }

    QList<QAction*> itemSpecificActions = ContextMenuUtil::getActionsForObject(schemaName(),
                                                                               itemName(),
                                                                               getItemType(),
                                                                               getModel()->getUiManager());

    if(!actions.isEmpty() && !itemSpecificActions.isEmpty()){
        QAction *separator=new QAction("-",0);
        separator->setSeparator(true);
        actions.append(separator);
    }

    actions.append(itemSpecificActions);

    return actions;
}

QMenu *DbTreeItem::getContextMenu(const QModelIndex &index) const
{
    QList<QAction*> actions=getContextMenuItems(index);

    if(actions.size()==0){
        return 0;
    }

    QMenu *menu=new QMenu();

    menu->addActions(actions);

    return menu;
}

DbTreeItem::CheckBehavior DbTreeItem::checkBehavior() const
{
    return Checkable;
}

void DbTreeItem::setCheckState(Qt::CheckState checkState)
{
    if(this->m_checkState!=checkState){
        this->m_checkState=checkState;
    }
}

Qt::CheckState DbTreeItem::checkState() const
{
    return this->m_checkState;
}

bool DbTreeItem::canGenerateDdlForItem() const
{
    return true;
}
