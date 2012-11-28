#include "synonymviewer.h"
#include "synonymviewertabs.h"

SynonymViewer::SynonymViewer(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *SynonymViewer::getViewerWidget() const
{
    return new SynonymViewerTabs(schemaName, objectName, uiManager);
}
