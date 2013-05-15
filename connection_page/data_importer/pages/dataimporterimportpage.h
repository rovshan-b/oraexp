#ifndef DATAIMPORTERIMPORTPAGE_H
#define DATAIMPORTERIMPORTPAGE_H

#include <QWizardPage>
#include "connectivity/ociexception.h"

class QLabel;
class IQueryScheduler;
class DataImporterThread;

class DataImporterImportPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterImportPage(QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual void initializePage();
    
private slots:
    void setStatus(const QString &status);
    void chunkImported(int chunkSize);
    void importCompleted();
    void importError(const QString &taskName, const OciException &ex);

private:
    QLabel *statusLabel;

    IQueryScheduler *queryScheduler;

    DataImporterThread *workerThread;

    int importedCount;

    void startWorkerThread();
    void deleteWorkerThread();
    
};

#endif // DATAIMPORTERIMPORTPAGE_H
