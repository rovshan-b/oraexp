#include "nodepopulatorthread.h"
#include "dbtreeitem.h"
#include "connectivity/ociexception.h"

#include <iostream>
using namespace std;

NodePopulatorThread::NodePopulatorThread(const QModelIndex &parentNode, QObject *parent) :
    QThread(parent), parentNode(parentNode)
{
}

NodePopulatorThread::~NodePopulatorThread()
{
    cout << "NodePopulatorThread deleted" << endl;
}

void NodePopulatorThread::run()
{
    DbTreeItemResult result;

    try{

        DbTreeItem *node=static_cast<DbTreeItem*>(parentNode.internalPointer());
        QList<DbTreeItem*> childNodeList = node->populateChildren();

        result.childNodes=childNodeList;

    }catch(OciException &ex){
        result.hasError=true;
        result.exception=ex;
    }

    result.parentNode=parentNode;

    emit queryCompleted(result);
}
