#ifndef CODECREATOR_H
#define CODECREATOR_H

#include "connection_page/connectionpagetab.h"
#include "navtree/dbtreemodel.h"
#include "codecreatorwidget.h"

class CodeCreator : public ConnectionPageTab
{
public:
    explicit CodeCreator(
            const QString &schemaName,
            const QString &objectName,
            DbTreeModel::DbTreeNodeType objectType,
            DbUiManager *uiManager, QWidget *parent = 0);

    virtual void createUi();
    virtual void setConnection(DbConnection *db);

    virtual void focusAvailable();

private:
    CodeCreatorWidget *creatorWidget;
};

#endif // CODECREATOR_H
