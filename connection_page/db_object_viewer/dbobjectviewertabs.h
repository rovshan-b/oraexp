#ifndef DBOBJECTVIEWERTABS_H
#define DBOBJECTVIEWERTABS_H

#include "widgets/genericqueryviewertabs.h"
#include "widgets/subtabwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreemodel.h"

class QMenu;

class DbObjectViewerTabs : public GenericQueryViewerTabs
{
    Q_OBJECT
public:
    explicit DbObjectViewerTabs(const QString &schemaName,
                                const QString &objectName,
                                DbTreeModel::DbTreeNodeType itemType,
                                DbUiManager *uiManager,
                                QWidget *parent);

    virtual ~DbObjectViewerTabs();

    void setHasSpecBodySwitcher(bool hasSpecBodySwitcher);

    void setCurrentTab(int ix);

signals:
    void specBodySwitchRequested();

protected:
    const QString schemaName;
    const QString objectName;
    DbTreeModel::DbTreeNodeType itemType;

    bool hasSpecBodySwitcher;

    QMenu *objectActionsMenu;

    virtual void initTab(GenericQueryViewerWidget *tab);
    virtual void createToolbarButtons();
};

#endif // DBOBJECTVIEWERTABS_H
