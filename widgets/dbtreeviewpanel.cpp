#include "dbtreeviewpanel.h"
#include "interfaces/iqueryscheduler.h"
#include "dialogs/selectlinkedobjectsdialog.h"
#include "dialogs/dbobjectselectordialog.h"
#include "navtree/dbtreeitem.h"
#include "navtree/dbtreemodelnodeexpander.h"
#include "util/dbutil.h"
#include <QtGui>

DbTreeViewPanel::DbTreeViewPanel(QWidget *parent) :
    QWidget(parent), selectLinkedObjectsDialog(0)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    dbTree=new DbTreeView();
    mainLayout->addWidget(dbTree);

    createActionButtons(mainLayout);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(dbTree, SIGNAL(treeViewReady()), this, SLOT(treeReady()));
}

void DbTreeViewPanel::createActionButtons(QBoxLayout *layout)
{
    QHBoxLayout *buttonsLayout=new QHBoxLayout();
    buttonsLayout->setContentsMargins(0,0,0,0);

    btnSelectAll=new QPushButton(tr("All"));
    btnSelectAll->setEnabled(false);
    connect(btnSelectAll, SIGNAL(clicked()), this, SLOT(selectAll()));
    buttonsLayout->addWidget(btnSelectAll);

    btnSelectNone=new QPushButton(tr("None"));
    btnSelectNone->setEnabled(false);
    connect(btnSelectNone, SIGNAL(clicked()), this, SLOT(selectNone()));
    buttonsLayout->addWidget(btnSelectNone);

    btnSelectLinkedObjects=new QPushButton(tr("Linked objects"));
    btnSelectLinkedObjects->setEnabled(false);
    connect(btnSelectLinkedObjects, SIGNAL(clicked()), this, SLOT(selectLinkedObjects()));
    buttonsLayout->addWidget(btnSelectLinkedObjects);

    buttonsLayout->addStretch();

    layout->addLayout(buttonsLayout);
}

void DbTreeViewPanel::setInProgress(bool inProgress)
{
    this->setEnabled(!inProgress);
}

void DbTreeViewPanel::selectAll()
{
    dbTree->checkAll(QModelIndex(), true);
}

void DbTreeViewPanel::selectNone()
{
    dbTree->checkAll(QModelIndex(), false);
}

void DbTreeViewPanel::selectLinkedObjects()
{
    QList<DbTreeModel::DbTreeNodeType> fullyCheckedObjectTypes;
    QList<DbTreeModel::DbTreeNodeType> uncheckedObjectTypes;

    DbTreeModel *model=dbTree->getModel();
    int topLevelRowCount = model->rowCount(QModelIndex());

    availableObjectTypes.clear();
    for(int i=0; i<topLevelRowCount; ++i){
        DbTreeItem *item = static_cast<DbTreeItem*>(model->index(i, 0, QModelIndex()).internalPointer());
        availableObjectTypes.append(item->getItemType());

        Qt::CheckState checkState = item->checkState();

        if(checkState==Qt::Checked){
            fullyCheckedObjectTypes.append(item->getItemType());
        }else if(checkState==Qt::Unchecked){
            uncheckedObjectTypes.append(item->getItemType());
        }
    }

    if(fullyCheckedObjectTypes.size()==topLevelRowCount || uncheckedObjectTypes.size()==topLevelRowCount){
        QString title = fullyCheckedObjectTypes.size()==topLevelRowCount ? tr("All items selected") : tr("Nothing selected");
        QString message = fullyCheckedObjectTypes.size()==topLevelRowCount ? tr("All items are already selected.") : tr("Please, select at least one item and try again.");

        QMessageBox::information(this->window(),
                                 title,
                                 message);

        return;
    }

    if(selectLinkedObjectsDialog==0){
        selectLinkedObjectsDialog = new SelectLinkedObjectsDialog(this);
        selectLinkedObjectsDialog->setAvailableObjectTypes(availableObjectTypes);
    }

    if(!selectLinkedObjectsDialog->exec()){
        return;
    }


    QList<DbTreeModel::DbTreeNodeType> selectedObjectTypes = selectLinkedObjectsDialog->getSelectedItemTypes();
    for(int i=0; i<fullyCheckedObjectTypes.size(); ++i){
        selectedObjectTypes.removeOne(fullyCheckedObjectTypes.at(i));
    }

    if(selectedObjectTypes.isEmpty()){
        QMessageBox::information(this->window(), tr("Nothing to find"),
                                 tr("Nothing to search for. Please, select more object types and try again."));
        return;
    }

    findOnlyTables = selectedObjectTypes.size()==1 && selectedObjectTypes.at(0)==DbTreeModel::Tables &&
                      !uncheckedObjectTypes.contains(DbTreeModel::Tables) && uncheckedObjectTypes.size()==topLevelRowCount-1;

    setInProgress(true);

    DbTreeModelNodeExpander *expander = new DbTreeModelNodeExpander(model);
    connect(expander, SIGNAL(loadComplete()), this, SLOT(childrenLoaded()));
    connect(expander, SIGNAL(loadError(OciException)), this, SLOT(childrenLoadError(OciException)));
    expander->loadChildren(QModelIndex(), availableObjectTypes);
}

void DbTreeViewPanel::treeReady()
{
    btnSelectAll->setEnabled(true);
    btnSelectNone->setEnabled(true);
    btnSelectLinkedObjects->setEnabled(true);
}

void DbTreeViewPanel::childrenLoaded()
{
    DbTreeModel *model=dbTree->getModel();

    QStringList objectNames, objectTypes;
    QStringList scanObjectTypes;

    for(int i=0; i<availableObjectTypes.size(); ++i){
        DbTreeModel::DbTreeNodeType selectedNodeType = availableObjectTypes.at(i);
        QString objectTypeName = DbUtil::getDbObjectTypeNameByNodeType(selectedNodeType);
        scanObjectTypes.append(objectTypeName);

        QList<QModelIndex> checkedIndexes = model->getCheckedGrandChildren(QModelIndex(), selectedNodeType);
        for(int k=0; k<checkedIndexes.size(); ++k){
            const QModelIndex &index = checkedIndexes.at(k);
            DbTreeItem *item = static_cast<DbTreeItem*>(index.internalPointer());
            if(item->canGenerateDdlForItem()){
                objectNames.append(item->itemName());
                objectTypes.append(objectTypeName);

                if(selectedNodeType==DbTreeModel::Packages){
                    objectNames.append(item->itemName());
                    objectTypes.append("PACKAGE BODY");
                }else if(selectedNodeType==DbTreeModel::Types){
                    objectNames.append(item->itemName());
                    objectTypes.append("TYPE BODY");
                }
            }
        }
    }

    QString schemaName=model->getDefaultSchemaName();
    QString nameLike = selectLinkedObjectsDialog->nameLikeEditor->text();
    QString nameNotLike = selectLinkedObjectsDialog->nameNotLikeEditor->text();
    int maxLevel = selectLinkedObjectsDialog->maxLevelEditor->value();
    int scanType = selectLinkedObjectsDialog->relationTypeComboBox->currentIndex();

    //bool stopOnInvalidType = selectLinkedObjectsDialog->strictMatchCheckBox->isChecked();
    bool stopOnInvalidType = false;

    IQueryScheduler *queryScheduler = dbTree->getQueryScheduler();
    queryScheduler->enqueueQuery("find_linked_objects",
                                              QList<Param*>()
                                              << new Param("obj_names", objectNames)
                                              << new Param("obj_types", objectTypes)
                                              << new Param("obj_count", objectNames.size())
                                              << new Param("scan_obj_types", scanObjectTypes)
                                              << new Param("scan_obj_count", scanObjectTypes.size())
                                              << new Param("owner", schemaName)
                                              << new Param("name_like", nameLike)
                                              << new Param("name_not_like", nameNotLike)
                                              << new Param("max_level", maxLevel)
                                              << new Param("scan_type", scanType)
                                              << new Param("only_tables", findOnlyTables)
                                              << new Param("stop_on_invalid_type", stopOnInvalidType),
                                              this,
                                              "find_linked_objects",
                                              "linkedObjectsQueryCompleted",
                                              "linkedObjectRecordAvailable",
                                              "linkedObjectsFetchCompleted",
                                              true);
}

void DbTreeViewPanel::childrenLoadError(const OciException &)
{
    this->setEnabled(false);
}

void DbTreeViewPanel::linkedObjectsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving linked objects"), result.exception.getErrorMessage());
        setInProgress(false);
        return;
    }
}

void DbTreeViewPanel::linkedObjectRecordAvailable(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching linked objects"), fetchResult.exception.getErrorMessage());
        return;
    }

    QString objects=fetchResult.oneRow.at(0);
    if(objects.isEmpty()){
        QMessageBox::information(this->window(), tr("Nothing found"),
                                 tr("No linked object found."));
    }else{
        QStringList nameTypePairs = objects.split(";", QString::SkipEmptyParts);
        QStringList objectNames;
        QStringList objectTypes;
        QStringList levels;

        QStringList parts;
        for(int i=0; i<nameTypePairs.size(); ++i){
            const QString &pair = nameTypePairs.at(i);
            parts=pair.split(",", QString::SkipEmptyParts);
            Q_ASSERT(parts.size()==3);

            objectNames.append(parts.at(0));
            objectTypes.append(parts.at(1));
            levels.append(parts.at(2));
        }

        DbObjectSelectorDialog dialog(this);
        dialog.setObjectList(objectNames, objectTypes, levels);
        dialog.exec();
    }
}

void DbTreeViewPanel::linkedObjectsFetchCompleted(const QString &)
{
    setInProgress(false);
}
