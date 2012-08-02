#include "nodeaction.h"
#include <iostream>

using namespace std;

NodeAction::NodeAction(const QModelIndex &index, const QIcon &icon, const QString &text, QObject *parent) :
    QAction(icon, text, parent)
{
    this->index=index;
}

QModelIndex NodeAction::getModelIndex() const
{
    return this->index;
}
