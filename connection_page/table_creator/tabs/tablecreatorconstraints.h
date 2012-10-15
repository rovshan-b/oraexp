#ifndef TABLECREATORCONSTRAINTS_H
#define TABLECREATORCONSTRAINTS_H

#include "../tablecreatortabwithtableview.h"
#include "interfaces/istringlistretriever.h"
#include "enums.h"
#include "util/param.h"
#include "connectivity/queryresult.h"
#include "beans/constraintinfo.h"
#include "widgets/datatable.h"
#include "models/tableconstraintsmodel.h"
#include "beans/tableinfo.h"
#include <QModelIndex>

class TableCreatorTabs;

class TableCreatorConstraints : public TableCreatorTabWithTableView,
                                public IStringListRetriever
{
    Q_OBJECT
public:
    explicit TableCreatorConstraints(const QString &schemaName,
                                     TableCreatorTabs *objectCreator,
                                     bool editMode,
                                     QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void currentSchemaChanged(const QString &oldSchemaName, const QString &newSchemaName);
    void setTableType(OraExp::TableType tableType);
    void setTableInfo(TableInfo *tableInfo);

    virtual QStringList getStringList(const QString &type);

    QList<ConstraintInfo> getConstraintsInfo() const;

signals:
    void ddlChanged() const;

private:
    void customizeTableWidget(const QString &schemaName);
    QString schemaName;

    //these variables and functions are only used in edit mode
    void populateTableWithConstraints();

    QList<ConstraintInfo> *originalConstraintList;

protected slots:
    virtual void showAdvancedOptions(bool show);

    //void beforeAlterTable(){static_cast<TableConstraintsModel*>(table->model())->removeIncorrectRows();}
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);

    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);

};

#endif // TABLECREATORCONSTRAINTS_H
