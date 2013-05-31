#ifndef TABLECREATORTABS_H
#define TABLECREATORTABS_H

#include "widgets/subtabwidget.h"
#include "enums.h"
#include "util/param.h"
#include "beans/querylistitem.h"
#include "beans/tableinfo.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class IQueryScheduler;
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
                              DbObjectCreator::CreatorMode creatorMode,
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
    void setTableName(const QString &tableName);
    QString getFullTableName() const;
    //used in edit mode
    QString getOriginalTableName() const;
    void setOriginalTableName(const QString &originalTableName);

    OraExp::TableType getTableType() const;

    void setQueryScheduler(IQueryScheduler *queryScheduler);
    IQueryScheduler *getQueryScheduler() const {return this->queryScheduler;}

    //used in edit mode
    //QList<Param*> getBasicQueryParams() const;

signals:
    void ddlChanged() const;

    void objectInfoLoaded();

private:
    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString originalTableName;

    DbObjectCreator::CreatorMode creatorMode;

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

    TableInfo *originalTableInfo;

private slots:
    void currentSchemaChanged();
    void tableTypeChanged(OraExp::TableType newTableType);

    void tableInfoReady(DbObjectInfo *tableInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);
};

#endif // TABLECREATORTABS_H
