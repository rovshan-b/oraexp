#ifndef TABLECREATORCHECKCONSTRAINTS_H
#define TABLECREATORCHECKCONSTRAINTS_H

#include "../tablecreatortabwithtableview.h"
#include "beans/constraintinfo.h"
#include "connectivity/queryresult.h"
#include "models/tablecheckconstraintsmodel.h"
#include "widgets/datatable.h"
#include "beans/tableinfo.h"

class DbConnection;

class TableCreatorCheckConstraints : public TableCreatorTabWithTableView
{
    Q_OBJECT
public:
    explicit TableCreatorCheckConstraints(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setTableInfo(TableInfo *tableInfo);

    QList<ConstraintInfo> getConstraintsInfo() const;

signals:
    void ddlChanged() const;

private:
    void customizeTableWidget();

    //these variables and functions are only used in edit mode
    void populateTableWithConstraints();

    QList<ConstraintInfo> *originalConstraintList;

protected slots:
    virtual void showAdvancedOptions(bool show);

    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);

    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);

};

#endif // TABLECREATORCHECKCONSTRAINTS_H
