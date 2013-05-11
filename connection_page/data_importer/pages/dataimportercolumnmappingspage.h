#ifndef DATAIMPORTERCOLUMNMAPPINGSPAGE_H
#define DATAIMPORTERCOLUMNMAPPINGSPAGE_H

#include <QWizardPage>
#include <QModelIndex>
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;
class DataTable;
class DataImporterColumnMappingsModel;
class QStandardItemModel;

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

    void mappingsModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    IQueryScheduler *querySheduler;

    DataTable *mappingsTable;
    DataImporterColumnMappingsModel *mappingsModel;

    DataTable *previewTable;

    QString currentSchemaName;
    QString currentTableName;
    QString currentFileName;

    void loadColumnList();
    void setFileFieldList(QStandardItemModel *model);

    QStringList getNonEmptyDates(int previewTableColIx) const;

    bool loadInProgress;
    
};

#endif // DATAIMPORTERCOLUMNMAPPINGSPAGE_H
