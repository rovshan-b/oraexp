#ifndef CODECREATOR_H
#define CODECREATOR_H

#include "../connectionpagetab.h"
#include "navtree/dbtreemodel.h"

class DbObjectInfo;
class MetadataLoader;
class CodeEditorAndSearchPaneWidget;

class CodeCreator : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit CodeCreator(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         DbUiManager *uiManager,
                         QWidget *parent = 0);

    virtual void createUi();
    virtual void setConnection(DbConnection *db);

signals:
    
private slots:
    void objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

private:
    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType objectType;

    bool editMode;

    CodeEditorAndSearchPaneWidget *editor;
    
};

#endif // CODECREATOR_H
