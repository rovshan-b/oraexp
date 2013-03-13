#include "dbtreeviewpanel.h"
#include "interfaces/iqueryscheduler.h"
#include "dialogs/selectlinkedtablesdialog.h"
#include "navtree/dbtreeitem.h"
#include <QtGui>

DbTreeViewPanel::DbTreeViewPanel(QWidget *parent) :
    QWidget(parent)
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

    btnSelectLinkedTables=new QPushButton(tr("Linked tables"));
    btnSelectLinkedTables->setEnabled(false);
    connect(btnSelectLinkedTables, SIGNAL(clicked()), this, SLOT(selectLinkedTables()));
    buttonsLayout->addWidget(btnSelectLinkedTables);

    buttonsLayout->addStretch();

    layout->addLayout(buttonsLayout);
}

void DbTreeViewPanel::selectAll()
{
    dbTree->checkAll(QModelIndex(), true);
}

void DbTreeViewPanel::selectNone()
{
    dbTree->checkAll(QModelIndex(), false);
}

void DbTreeViewPanel::selectLinkedTables()
{
    //IQueryScheduler *queryScheduler = dbTree->getQueryScheduler();
    QModelIndex tablesIndex = dbTree->getModel()->getChildIndex(QModelIndex(), DbTreeModel::Tables);
    Q_ASSERT(tablesIndex.isValid());
    DbTreeItem *tablesItem = static_cast<DbTreeItem*>(tablesIndex.internalPointer());
    Q_ASSERT(tablesItem);
    Qt::CheckState checkState = tablesItem->checkState();

    if(checkState==Qt::Checked){
        QMessageBox::information(this->window(),
                                 tr("Already selected"),
                                 tr("All tables are already selected."));
        return;
    }else if(checkState==Qt::Unchecked){
        QMessageBox::information(this->window(),
                              tr("Nothing selected"),
                              tr("Please, select at least one table and try again."));
        return;
    }

    SelectLinkedTablesDialog dialog(this);
    if(dialog.exec()){
        //dbTree->getModel()->getCheckedItems(QModelIndex(), DbTreeModel::Tables);
    }
}

void DbTreeViewPanel::treeReady()
{
    btnSelectAll->setEnabled(true);
    btnSelectNone->setEnabled(true);
    btnSelectLinkedTables->setEnabled(true);
}
