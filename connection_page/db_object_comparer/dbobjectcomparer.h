#ifndef DBOBJECTCOMPARER_H
#define DBOBJECTCOMPARER_H

#include "../connectionpagetab.h"
#include "tabs/dbobjectcomparerresultstab.h"
#include "connectivity/ociexception.h"
#include "beans/dbobjectcomparisonoptions.h"
#include <QTime>
#include <QTabWidget>

class IDbObjectCompareHelper;
class DbObjectComparerCompareTab;
class DbObjectComparerOptionsTab;
class QLabel;
class QProgressBar;
class QPushButton;
class DbTreeModel;
class QBoxLayout;

class DbObjectComparer : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit DbObjectComparer(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void createUi();
    
    virtual void setConnection(DbConnection *db);

    virtual bool canFind() const;

    virtual void showSearchWidget();
    virtual void findNext();
    virtual void findPrevious();

public slots:
    void startStopComparing();

    void statusChanged(const QString &newStatus);
    void comparisonResultAvailable(const QString &ddl);
    void completed();
    void comparisonError(const QString &taskName, const OciException &exception);
    void objectCountDetermined(int count);
    void chunkCompleted(int size);

    void targetConnectionEstablished(DbConnection *db);

private slots:
    void createTabs();
    void busyStateChanged(bool busy);

protected:
    DbObjectComparerCompareTab* createCompareTab(DbUiManager *uiManager);
    DbObjectComparerOptionsTab *createOptionsTab();
    DbObjectComparerResultsTab *createResultsTab();

    virtual DbObjectComparerCompareTab *doCreateCompareTab(DbUiManager *uiManager)=0;
    virtual DbObjectComparerOptionsTab *doCreateOptionsTab()=0;
    virtual DbObjectComparerResultsTab *doCreateResultsTab()=0;

    virtual IDbObjectCompareHelper *createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                                        const QString &targetSchema, IQueryScheduler *targetScheduler,
                                                        DbObjectComparisonOptions *options, QObject *parent)=0;

    void createBottomLayout(QBoxLayout *layout);
    void showStatusControls(bool show);
    void enableControls(bool enable);

    bool isInProgress() const;

    void startComparing();
    void stopComparing();

    void deleteCompareHelper();

    IDbObjectCompareHelper *comparer;

    QTabWidget *tabWidget;

    DbObjectComparerCompareTab *compareTab;
    DbObjectComparerOptionsTab *optionsTab;
    DbObjectComparerResultsTab *resultsTab;

    QLabel *statusLabel;
    QProgressBar *progressBar;

    QPushButton *compareButton;

    QTime timer;
    
};

#endif // DBOBJECTCOMPARER_H
