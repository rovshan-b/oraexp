#include "sequenceviewer.h"
#include "sequenceviewertabs.h"

SequenceViewer::SequenceViewer(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *SequenceViewer::getViewerWidget() const
{
    return new SequenceViewerTabs(schemaName, objectName, this->uiManager);
}
