#ifndef DBOBJECTVIEWERTABS_H
#define DBOBJECTVIEWERTABS_H

#include "widgets/genericqueryviewertabs.h"
#include "widgets/subtabwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreemodel.h"

class DbObjectViewerTabs : public GenericQueryViewerTabs
{
    Q_OBJECT
public:
    explicit DbObjectViewerTabs(const QString &schemaName,
                                const QString &objectName,
                                DbTreeModel::DbTreeNodeType itemType,
                                DbUiManager *uiManager,
                                QWidget *parent);

    void setHasSpecBodySwitcher(bool hasSpecBodySwitcher);

signals:
    void specBodySwitchRequested();

protected:
    const QString schemaName;
    const QString objectName;
    DbTreeModel::DbTreeNodeType itemType;

    bool hasSpecBodySwitcher;

    virtual void initTab(GenericQueryViewerWidget *tab);
    virtual void createToolbarButtons();
};

#endif // DBOBJECTVIEWERTABS_H
