#ifndef CODEVIEWER_H
#define CODEVIEWER_H

#include "../db_object_viewer/dbobjectviewer.h"

class CodeViewer : public DbObjectViewer
{
    Q_OBJECT
public:
    explicit CodeViewer(const QString &schemaName,
                        const QString &objectName,
                        DbTreeModel::DbTreeNodeType itemType,
                        DbUiManager *uiManager,
                        QWidget *parent = 0);
    
    virtual DbObjectViewerTabs *getTabsWidget() const;

private:
    DbTreeModel::DbTreeNodeType itemType;
    
};

#endif // CODEVIEWER_H
