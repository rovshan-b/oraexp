#ifndef TABS_H
#define TABLECREATORTABS_H

#include "widgets/subtabwidget.h"
#include "enums.h"
#include "util/param.h"
#include "beans/querylistitem.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/tableinfo.h"

class DbConnection;
class TableCreatorGeneralInfo;
class TableCreatorColumns;
class TableCreatorConstraints;
class TableCreatorCheckConstraints;
class TableCreatorIndexes;
class TableCreatorPartitions;
class TableCreatorGrants;
class TableCreatorExternalProperties;
class QScrollArea;
class MetadataLoader;

class TableCreatorTabs : public SubTabWidget
{
    Q_OBJECT
public:
    explicit TableCreatorTabs(const QString &schemaName,
                              const QString &originalTableName,
                              QWidget *parent = 0);

    virtual ~TableCreatorTabs();

    TableCreatorGeneralInfo *getGeneralInfoTab();
    TableCreatorColumns *getColumnsTab();
    TableCreatorConstraints *getConstraintsTab();

    TableInfo getTableInfo() const;
    QList< QueryListItem > generateAlterDdl();
    void removeIncorrectRows();
    bool beforeAlterTable();

    QString getSchemaName() const;
    QString getTableName() const;
    //used in edit mode
    QString getOriginalTableName() const;
    void setOriginalTableName(const QString &originalTableName);

    OraExp::TableType getTableType() const;

    void setConnection(DbConnection *db);

    IQueryScheduler *scheduler() const {return this->queryScheduler;}

    //used in edit mode
    //QList<Param*> getBasicQueryParams() const;

signals:
    void ddlChanged() const;
    void alterQuerySucceeded(const QString &taskName);

    void objectInfoLoaded();

private:
    DbConnection *db;
    QString schemaName;
    QString originalTableName;

    bool editMode;

    TableCreatorGeneralInfo *generalInfoTab;
    TableCreatorColumns *columnsTab;
    TableCreatorConstraints *keyConstraintsTab;
    TableCreatorCheckConstraints *checkConstraintsTab;
    TableCreatorIndexes *indexesTab;
    TableCreatorPartitions *partitionsTab;
    QScrollArea *scrollAreaForPartitionsTab;
    TableCreatorExternalProperties *externalTableProps;
    QScrollArea *scrollAreaForExternalTableProps;
    TableCreatorGrants *grantsTab;

    void showTabsBasedOnTableType(OraExp::TableType tableType);

    IQueryScheduler *queryScheduler;

    TableInfo *originalTableInfo;

private slots:
    void currentSchemaChanged();
    void tableTypeChanged(OraExp::TableType newTableType);

    void tableInfoReady(DbObjectInfo *tableInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);
};

#endif // TABLECREATORTABS_H
