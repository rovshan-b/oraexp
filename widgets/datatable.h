#ifndef DATATABLE_H
#define DATATABLE_H

#include <QTableView>
#include "util/param.h"
#include "connectivity/queryresult.h"

class DbConnection;
class Resultset;
class DbConnection;
class IQueryScheduler;
class StatementDesc;

class DataTable : public QTableView
{
    Q_OBJECT
public:
    explicit DataTable(QWidget *parent = 0);
    void setResultset(IQueryScheduler *queryScheduler, Resultset *rs, QHash<int, StatementDesc*> dynamicQueries=QHash<int, StatementDesc*>());

    void displayQueryResults(IQueryScheduler *queryScheduler, const QString &query, QList<Param*> params,
                             QHash<int, StatementDesc*> dynamicQueries=QHash<int, StatementDesc*>());

    void clear();

    void setIconColumn(const QString &displayColumnName, const QString &iconColumnName);

    void setHumanizeColumnNames(bool humanize){this->humanizeColumnNames=humanize;}

    void resizeColumnAccountingForEditor(int column);
    void resizeColumnsAccountingForEditor();

public slots:
    void resizeColumnsToFitContents();

signals:
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void asyncQueryCompleted(const QueryResult &result);
    void asyncQueryError(const OciException &ex);
    void firstFetchCompleted();

protected:
    void keyPressEvent ( QKeyEvent * event );

private slots:
    void queryCompleted(const QueryResult &result);

private:
    IQueryScheduler *queryScheduler;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<QString, QString> iconColumns;

    bool humanizeColumnNames;

    void copyToClipboard();
};

#endif // DATATABLE_H
