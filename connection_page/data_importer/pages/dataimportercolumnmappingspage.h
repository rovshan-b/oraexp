#ifndef DATAIMPORTERCOLUMNMAPPINGSPAGE_H
#define DATAIMPORTERCOLUMNMAPPINGSPAGE_H

#include <QWizardPage>
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;
class DataTable;
class DataImporterColumnMappingsModel;

class DataImporterColumnMappingsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterColumnMappingsPage(QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual void initializePage();

private slots:
    void columnsQueryCompleted(const QueryResult &result);
    void columnFetched(const FetchResult &fetchResult);
    void columnFetchCompleted(const QString &);

private:
    IQueryScheduler *querySheduler;

    DataTable *mappingsTable;
    DataImporterColumnMappingsModel *mappingsModel;

    DataTable *previewTable;

    QString currentSchemaName;
    QString currentTableName;
    QString currentFileName;
    
};

#endif // DATAIMPORTERCOLUMNMAPPINGSPAGE_H
