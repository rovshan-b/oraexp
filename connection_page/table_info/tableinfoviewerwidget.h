#ifndef TABLEINFOVIEWERWIDGET_H
#define TABLEINFOVIEWERWIDGET_H

#include "widgets/ondemandinfoviewerwidget.h"
#include "interfaces/iqueryscheduler.h"

class DbConnection;
class TableInfoToolbar;

class TableInfoViewerWidget : public OnDemandInfoViewerWidget
{
public:
    TableInfoViewerWidget(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    virtual void setTableName(const QString &schemaName, const QString &tableName);

    virtual void createMainWidget(QLayout *);

    void createChildControls();
    bool areControlsCreated() const {return this->controlsCreated;}

protected:
    void createToolbarButtons();

    virtual void addSpecificToolbarButtons();

    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString tableName;

    TableInfoToolbar *toolbar;
    QAction *refreshButton;
    QAction *progressBarAction;

    bool controlsCreated;
};

#endif // TABLEINFOVIEWERWIDGET_H
