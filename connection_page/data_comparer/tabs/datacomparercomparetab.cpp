#include "datacomparercomparetab.h"
#include "widgets/dbtreeview.h"
#include "navtree/dbtreeitem.h"
#include "navtree/table/dbtablecolumnitem.h"
#include "util/dbutil.h"
#include "util/strutil.h"
#include "defines.h"
#include <QtGui>

DataComparerCompareTab::DataComparerCompareTab(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparerCompareTab(uiManager, parent)
{
    schemaObjectsTree=new DbTreeView();
    rightPane=new QWidget();
    rightPane->setEnabled(false);

    supportedDataTypes.append(DbUtil::getComparableDataTypes());
}

void DataComparerCompareTab::createItemsTable(QBoxLayout *layout)
{
    QSplitter *splitter=new QSplitter(Qt::Horizontal);

    //splitter->addWidget(schemaObjectsTree);
    QLabel *treeLabel=new QLabel(tr("Tables to compare:"));
    QWidget *treeParent=new QWidget();
    QLayout *treeLayout=new QVBoxLayout();
    treeLayout->setContentsMargins(0,0,0,0);
    treeLayout->addWidget(treeLabel);
    treeLayout->addWidget(schemaObjectsTree);
    treeParent->setLayout(treeLayout);
    splitter->addWidget(treeParent);

    QLabel *editorLabel=new QLabel(tr("Optional WHERE clause:"));
    QVBoxLayout *editorLayout=new QVBoxLayout();

    editorLayout->setContentsMargins(0,0,0,0);
    editorLayout->addWidget(editorLabel);
    whereClauseEditor=new CodeEditorAndSearchPaneWidget();
    editorLayout->addWidget(whereClauseEditor);

    QFormLayout *form=new QFormLayout();
    targetTablenameEditor=new QLineEdit();
    form->addRow(tr("Target table name:"), targetTablenameEditor);
    editorLayout->addLayout(form);

    rightPane->setLayout(editorLayout);

    splitter->addWidget(rightPane);

    layout->addWidget(splitter);
    layout->setStretchFactor(splitter, 1);
}

DbTreeModel::DbTreeNodeTypes DataComparerCompareTab::getNodeTypesToDisplay() const
{
    DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay;
    nodeTypesToDisplay.append(DbTreeModel::DbTreeNodeTypes() << DbTreeModel::Tables);
    return nodeTypesToDisplay;
}

DbTreeModel::DbTreeNodeTypes DataComparerCompareTab::getCheckableNodeTypes() const
{
    DbTreeModel::DbTreeNodeTypes checkableNodeTypes;
    checkableNodeTypes.append(DbTreeModel::DbTreeNodeTypes() << DbTreeModel::Tables << DbTreeModel::Table << DbTreeModel::Columns << DbTreeModel::Column);
    return checkableNodeTypes;
}

QHash<QString, TableInfoForDataComparison> DataComparerCompareTab::getTableOptions() const
{
    return tableOptions;
}

bool DataComparerCompareTab::isCheckable(DbTreeItem *item)
{
    if(item->getItemType()==DbTreeModel::Column){
        DbTableColumnItem *colItem=static_cast<DbTableColumnItem*>(item);
        return hasMatch(colItem->dataType(), supportedDataTypes);
    }

    return true;
}

void DataComparerCompareTab::treeChildrenPopulated(const QModelIndex &parent)
{
    if(!parent.isValid()){
        return;
    }
    DbTreeItem *item=static_cast<DbTreeItem *>(parent.internalPointer());

    Q_ASSERT(item);
    if(item->getItemType()==DbTreeModel::Schema){
        DbTreeItem *childItem;
        int childCount=parent.model()->rowCount(parent);
        for(int i=0; i<childCount; ++i){
            childItem=static_cast<DbTreeItem *>(parent.child(i,0).internalPointer());
            if(childItem->getItemType()==DbTreeModel::Tables){
                childItem->setCheckState(Qt::Unchecked);
                schemaObjectsTree->setExpanded(parent.child(i,0), true);
                break;
            }
        }

        item->getModel()->setCheckStateProvider(this);
    }
}

void DataComparerCompareTab::commitWhereQuery(const QModelIndex &index)
{
    DbTreeItem *prevItem=static_cast<DbTreeItem *>(index.internalPointer());
    if(prevItem && prevItem->getItemType()==DbTreeModel::Table){
        QString query=whereClauseEditor->editor()->toPlainText().trimmed();
        QString targetTableName=targetTablenameEditor->text().trimmed().toUpper();
        QString key=prevItem->itemName();
        if(query.isEmpty() && targetTableName.isEmpty() && tableOptions.contains(key)){
            tableOptions.remove(key);
        }else if(!query.isEmpty() || !targetTableName.isEmpty()){
            TableInfoForDataComparison info;
            info.whereClause=query;
            info.targetTableName=targetTableName;
            tableOptions[key]=info;
        }
    }
}

void DataComparerCompareTab::currentTreeItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
    commitWhereQuery(previous);

    if(!current.isValid()){
        whereClauseEditor->editor()->setPlainText("");
        targetTablenameEditor->setText("");
        rightPane->setEnabled(false);
        return;
    }

    DbTreeItem *currItem=static_cast<DbTreeItem *>(current.internalPointer());
    Q_ASSERT(currItem);
    if(currItem->getItemType()!=DbTreeModel::Table || !currItem->isTopLevel()){
        whereClauseEditor->editor()->setPlainText("");
        targetTablenameEditor->setText("");
        rightPane->setEnabled(false);
        return;
    }

    rightPane->setEnabled(true);
    TableInfoForDataComparison info=tableOptions.value(currItem->itemName());
    whereClauseEditor->editor()->setPlainText(info.whereClause);
    targetTablenameEditor->setText(!info.targetTableName.isEmpty() ? info.targetTableName : currItem->itemName());
    //targetTablenameEditor->setSelection(0,0);
    targetTablenameEditor->setCursorPosition(0);
}

void DataComparerCompareTab::beforeCompare()
{
    commitWhereQuery(schemaObjectsTree->currentIndex());
}
