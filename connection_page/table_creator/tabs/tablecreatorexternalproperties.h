#ifndef TABLECREATOREXTERNALPROPERTIES_H
#define TABLECREATOREXTERNALPROPERTIES_H

#include "../tablecreatortab.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/tableinfo.h"
#include "defines.h"

class QTabWidget;
class IStringListRetriever;
class TableCreatorExternalPropertiesGeneralInfoWidget;
class OracleLoaderDriverRecordPropertiesWidget;
class OracleLoaderDriverFieldPropertiesWidget;
class OracleDataPumpDriverProperties;
class TableCreatorExternalAccessParametersEditorWidget;

class TableCreatorExternalProperties : public TableCreatorTab
{
    Q_OBJECT
public:
    explicit TableCreatorExternalProperties(IQueryScheduler *queryScheduler,
                                            IStringListRetriever *columnListRetriever,
                                            TableCreatorTabs *objectCreator,
                                            DbObjectCreator::CreatorMode creatorMode,
                                            QWidget *parent);
    virtual ~TableCreatorExternalProperties();

    void setTableInfo(TableInfo *tableInfo);

    TableExternalInfo getExternalInfo() const;

signals:
    void ddlChanged();

private slots:
    void driverChanged(int selectedIndex);

    void dirListQueryCompleted(const QueryResult &result);
    void dirRecordFetched(const FetchResult &result);
    void dirListFetchCompleted(const QString &/*taskName*/);

    //used in edit mode
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &/*taskName*/){}

private:
    void populateDirectoryList();

    QStringList directoryList;

    QTabWidget *tab;
    TableCreatorExternalPropertiesGeneralInfoWidget *generalInfo;
    OracleLoaderDriverRecordPropertiesWidget *recordProperties;
    OracleLoaderDriverFieldPropertiesWidget *fieldProperties;
    OracleDataPumpDriverProperties *dataPumpProperties;

    //used in edit mode
    TableCreatorExternalAccessParametersEditorWidget *accessParamsEditor;
};

#endif // TABLECREATOREXTERNALPROPERTIES_H
