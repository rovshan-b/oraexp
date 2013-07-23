#include "connectionpageconnectwidget.h"
#include "widgets/treeview.h"
#include "widgets/connectioneditor.h"
#include <QtGui>

ConnectionPageConnectWidget::ConnectionPageConnectWidget(QWidget *parent) :
    QWidget(parent)
{
    createUi();
}

void ConnectionPageConnectWidget::createUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout();

    QSplitter *splitter = new QSplitter();
    splitter->setChildrenCollapsible(false);

    createConnectionList(splitter);
    createConnectionEditor(splitter);

    mainLayout->addWidget(splitter);
    setLayout(mainLayout);
}

void ConnectionPageConnectWidget::createConnectionList(QSplitter *splitter)
{
    QTreeView *tree = new TreeView();
    tree->setRootIsDecorated(false);
    QStandardItemModel *model = new QStandardItemModel(10, 5);
    tree->setModel(model);

    splitter->addWidget(tree);
}

void ConnectionPageConnectWidget::createConnectionEditor(QSplitter *splitter)
{
    ConnectionEditor *connectionEditor = new ConnectionEditor();

    splitter->addWidget(connectionEditor);
}
