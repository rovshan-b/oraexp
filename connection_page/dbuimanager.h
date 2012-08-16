#ifndef DBUIMANAGER_H
#define DBUIMANAGER_H

#include <QObject>

class DbConnection;
class ConnectionPage;

class DbUiManager : public QObject
{
    Q_OBJECT
public:
    explicit DbUiManager(DbConnection *db, QObject *parent = 0);

signals:

public slots:
    void refreshTreeNodeChildren();

    void viewTableDetails();
    void showTableCreator();

    void showViewCreator();

    void showProcedureCreator();

    void addWorksheet(const QString &contents="");

    void viewTableDetails(const QString &schemaName, const QString &tableName);
    void showTableCreator(const QString &schemaName, const QString &tableName);

    void showViewCreator(const QString &schemaName, const QString &viewName);

    void showProcedureCreator(const QString &schemaName, const QString &procName);

    void addSchemaComparer();
    void addDataComparer();

    void alterTable();
    void alterView();
    void alterProcedure();

    void closeTab(QWidget *widget);
private:
    DbConnection *db;
    ConnectionPage *cnPage;

    QString getCorrectSchemaNameForCurrentContext();
};

#endif // DBUIMANAGER_H
