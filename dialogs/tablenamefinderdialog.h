#ifndef TABLENAMEFINDERDIALOG_H
#define TABLENAMEFINDERDIALOG_H

#include "connection_page/connectionpagewindow.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class TableNameFinderDialog : public ConnectionPageWindow
{
    Q_OBJECT
public:
    explicit TableNameFinderDialog(const QString &schemaName,
                                   const QString &tabName,
                                   const QString &childObjectName,
                                   DbUiManager *uiManager,
                                   QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

    virtual bool initiallyVisible() const{return false;}
    
private slots:
    void findTableNameQueryCompleted(const QueryResult &result);
    void findTableNameRecordFetched(const FetchResult &result);
    void findTableNameFetchCompleted(const QString &taskName);

private:
    QString schemaName;
    QString tableName;
    QString tabName;
    QString childObjectName;
    
};

#endif // TABLENAMEFINDERDIALOG_H
