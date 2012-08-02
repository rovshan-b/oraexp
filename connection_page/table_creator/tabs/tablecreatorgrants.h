#ifndef TABLECREATORGRANTS_H
#define TABLECREATORGRANTS_H

#include "../tablecreatortabwithtableview.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/grantinfo.h"
#include "models/tablegrantsmodel.h"
#include "widgets/datatable.h"
#include "beans/tableinfo.h"

class DbConnection;
class SchemaSelectorDelegate;

class TableCreatorGrants : public TableCreatorTabWithTableView
{
    Q_OBJECT
public:
    explicit TableCreatorGrants(const QString &schemaName,
                                TableCreatorTabs *tableCreator,
                                bool editMode,
                                QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

    void currentSchemaChanged(const QString &oldSchemaName, const QString &newSchemaName);

    void setTableInfo(TableInfo *tableInfo);

    QList<GrantInfo> getGrantsInfo() const;

signals:
    void ddlChanged() const;

private:
    QString schemaName;

    SchemaSelectorDelegate *schemaListDelegate;

    void customizeTableWidget(const QString &schemaName);
    QString generateGrantStatement(const QString &tableName, const QString &privilegeName, const QString &granteeName, int grantType) const;

    //these functions and variables used in edit mode
    void populateTableWithGrants();

    QList<GrantInfo> *originalGrantList;

protected slots:
    virtual void showAdvancedOptions(bool show);

    //for edit mode only
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &/*taskName*/){}

    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);
};

#endif // TABLECREATORGRANTS_H
