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
class DbUiManager;
class ResultsetTableModel;

class DataTable : public QTableView
{
    Q_OBJECT
public:
    explicit DataTable(QWidget *parent = 0);
    void setResultset(IQueryScheduler *queryScheduler,
                      Resultset *rs,
                      QHash<int, StatementDesc*> dynamicQueries=QHash<int, StatementDesc*>());

    void displayQueryResults(IQueryScheduler *queryScheduler, const QString &query, QList<Param*> params,
                             QHash<int, StatementDesc*> dynamicQueries=QHash<int, StatementDesc*>());

    void clear();

    void setIconColumn(const QString &displayColumnName, const QString &iconColumnName);
    void setIconColumns(const QHash<QString, QString> &iconColumns);

    void setHumanizeColumnNames(bool humanize){this->humanizeColumnNames=humanize;}

    void resizeColumnAccountingForEditor(int column);
    void resizeColumnsAccountingForEditor();

    void setUiManager(DbUiManager *uiManager);
    void setObjectListMode(int schemaNameCol,
                           int objectNameCol,
                           int objectTypeCol,
                           const QString &objectListSchemaName=QString(),
                           const QString &objectListObjectType=QString());

public slots:
    void resizeColumnsToFitContents();

signals:
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void asyncQueryCompleted(const QueryResult &result);
    void asyncQueryError(const OciException &ex);
    void firstFetchCompleted();

protected:
    void keyPressEvent ( QKeyEvent * event );
    //void resizeEvent ( QResizeEvent * event );

private slots:
    void queryCompleted(const QueryResult &result);
    void showContextMenu(const QPoint &pos);
    void handleFirstFetchCompleted();

private:
    IQueryScheduler *queryScheduler;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<QString, QString> iconColumns;

    bool humanizeColumnNames;
    bool quietMode;

    void copyToClipboard();

    QString objectListSchemaName;
    QString objectListObjectType;
    int schemaNameCol;
    int objectNameCol;
    int objectTypeCol;

    DbUiManager *uiManager;

    void deleteCurrentModel();
    void displayError(const QString &prefix, const OciException &ex);
    int getVisibleRecordCount() const;
};

#endif // DATATABLE_H
