#ifndef DATAIMPORTERIMPORTPAGE_H
#define DATAIMPORTERIMPORTPAGE_H

#include "connection_page/connectionpagewizardpage.h"
#include <QTime>
#include "connectivity/ociexception.h"

class QLabel;
class IQueryScheduler;
class DataImporterThread;

class DataImporterImportPage : public ConnectionPageWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterImportPage(DbUiManager *uiManager, QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual void initializePage();

    virtual bool isBusy() const;
    
private slots:
    void setStatus(const QString &status);
    void chunkImported(int chunkSize);
    void importCompleted();
    void importError(const QString &taskName, const OciException &ex);

    void enableBackButton(bool enable = false);
    void enableNextButton(bool enable = false);
    void enableCancelButton(bool enable = false);

    void stopButtonPressed();

private:
    QLabel *statusLabel;
    QPushButton *stopButton;

    IQueryScheduler *queryScheduler;

    DataImporterThread *workerThread;

    long importedCount;
    QTime timer;

    void startWorkerThread();
    void deleteWorkerThread();
    
};

#endif // DATAIMPORTERIMPORTPAGE_H
