#ifndef DATATABLEEDITCONTROLLER_H
#define DATATABLEEDITCONTROLLER_H

#include <QObject>
#include "widgets/datatable.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class DataTableEditController : public QObject
{
    Q_OBJECT
public:
    explicit DataTableEditController(DataTable *parent);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setObjectName(const QString &schemaName, const QString &objectName, const QString &dblinkName);

    QList<QAction *> createEditActions();
    void enableEditActions(bool enable);


    void clearColumnDelegates();

signals:
    void refreshRequired();

public slots:
    void addRecord();
    void deleteRecord();

    void commit();
    bool reset();

    void showDml();

private slots:
    void loadConstraintInfo();

    void constraintsQueryCompleted(const QueryResult &result);
    void constraintFetched(const FetchResult &fetchResult);
    void constraintsFetchCompleted(const QString &);

    void setColumnDelegates();

    void commitQueryCompleted(const QueryResult &result);

private:
    DataTable *dt;
    IQueryScheduler *queryScheduler;

    QString commitDml;

    QAction *addAction;
    QAction *deleteAction;
    QAction *commitAction;
    QAction *rollbackAction;
    QAction *showDmlAction;
    
    QString schemaName;
    QString objectName;
    QString dblinkName;

    bool editingEnabled;
};

#endif // DATATABLEEDITCONTROLLER_H
