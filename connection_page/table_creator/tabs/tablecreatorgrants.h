#ifndef TABLECREATORGRANTS_H
#define TABLECREATORGRANTS_H

#include "../tablecreatortabwithtableview.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "widgets/datatable.h"
#include "beans/tableinfo.h"
#include "models/objectgrantsmodel.h"

class DbConnection;
class SchemaSelectorDelegate;
class ObjectGrantsEditorTable;

class TableCreatorGrants : public DbObjectCreatorTab<TableCreatorTabs>
{
    Q_OBJECT
public:
    explicit TableCreatorGrants(const QString &schemaName,
                                TableCreatorTabs *objectCreator,
                                DbObjectCreator::CreatorMode creatorMode,
                                QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void currentSchemaChanged(const QString &oldSchemaName, const QString &newSchemaName);

    void setTableInfo(TableInfo *tableInfo);

    void removeIncorrectRows();

    QList<ObjectGrantInfo> getGrantsInfo() const;

signals:
    void ddlChanged() const;

private:
    QString schemaName;

    ObjectGrantsEditorTable *grantsEditor;
    DataTable *table;

protected slots:
    virtual void showAdvancedOptions(bool show);
};

#endif // TABLECREATORGRANTS_H
