#include "paircodeviewer.h"
#include "codeviewer.h"

PairCodeViewer::PairCodeViewer(const QString &schemaName,
                               const QString &objectName,
                               DbTreeModel::DbTreeNodeType itemType,
                               DbUiManager *uiManager,
                               QWidget *parent) :
    ConnectionPageTab(parent)
{

}
