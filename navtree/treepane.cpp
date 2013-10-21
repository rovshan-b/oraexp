#include "treepane.h"
#include "widgets/dbtreeview.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include <QtGui>

TreePane::TreePane(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
    this->requiresSeparateConnection = false;

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);

    dbTree=new DbTreeView();

    QVBoxLayout *treeLayout=new QVBoxLayout();
    treeLayout->addWidget(dbTree);

    treeLayout->setContentsMargins(2,0,0,0);
    treeLayout->setSpacing(5);

    setLayout(treeLayout);
}

void TreePane::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    populateTree();

    //decreaseRefCount();
}

QSize TreePane::sizeHint() const
{
    return QSize(200, 200);
}

QString TreePane::getDisplayName() const
{
    return tr("Objects tree");
}

QIcon TreePane::getIcon() const
{
    return IconUtil::getIcon("tree");
}

void TreePane::populateTree()
{
    dbTree->setQueryScheduler(this, this->db->getSchemaName(), this->uiManager);
    dbTree->getModel()->setupInitialItems();
}
