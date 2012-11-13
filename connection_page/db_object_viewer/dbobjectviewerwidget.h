#ifndef DBOBJECTVIEWERWIDGET_H
#define DBOBJECTVIEWERWIDGET_H

#include "widgets/ondemandinfoviewerwidget.h"
#include "interfaces/iqueryscheduler.h"

class DbConnection;
class QToolBar;

class DbObjectViewerWidget : public OnDemandInfoViewerWidget
{
public:
    DbObjectViewerWidget(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    virtual void setObjectName(const QString &schemaName, const QString &tableName);

    virtual void createMainWidget(QLayout *) = 0;

    void createChildControls();
    bool areControlsCreated() const {return this->controlsCreated;}

protected:
    void createToolbarButtons();

    virtual void addSpecificToolbarButtons();

    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString tableName;

    QToolBar *toolbar;
    QAction *refreshButton;
    QAction *progressBarAction;

    bool controlsCreated;
};

#endif // DBOBJECTVIEWERWIDGET_H