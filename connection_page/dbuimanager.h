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

    void addWorksheet(const QString &contents="");
    void viewTableDetails(const QString &schemaName, const QString &tableName);
    void showTableCreator(const QString &schemaName, const QString &tableName);
    void addSchemaComparer();
    void addDataComparer();

    void alterTable();

    void addProgramUnitEditor();

    void closeTab(QWidget *widget);
private:
    DbConnection *db;
    ConnectionPage *cnPage;

};

#endif // DBUIMANAGER_H
