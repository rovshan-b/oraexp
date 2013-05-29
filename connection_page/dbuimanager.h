#ifndef DBUIMANAGER_H
#define DBUIMANAGER_H

#include <QObject>
#include "../navtree/dbtreemodel.h"
#include "enums.h"

class DbConnection;
class ConnectionPage;
class DbItemAction;
class ConnectionPageObject;
class ConnectionPageTab;
class Worksheet;

class DbUiManager : public QObject
{
    Q_OBJECT
public:
    explicit DbUiManager(DbConnection *db, QObject *parent = 0);

signals:

public slots:
    void refreshTreeNodeChildren();

    void createCreator();
    void createCreator(DbItemAction *action);
    void createCreator(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType);
    void createEditor(bool editMode=true);
    void createEditor(const QString &schemaName,
                      const QString &objectName,
                      const DbTreeModel::DbTreeNodeType itemType,
                      bool editMode=true,
                      QHash<QString, QString> properties = QHash<QString, QString>());
    void createViewer();
    void createViewer(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType);

    void addEditor(const QString &schemaName, const QString &objectName, const QString &objectTypeName,
                   const QString &tabName, const QString &childObjectName);

    Worksheet *addWorksheet(const QString &contents="");
    Worksheet *getWorksheet();
    void addSchemaComparer();
    void addSchemaExporter();
    void addDataComparer();
    void addDataCopier();
    void addDataExporter();
    void setProperties(ConnectionPageTab *tab);
    void addDataImporter();
    void addDataImporter(const QString &schemaName, const QString &tableName);

    void addDmlGenerator(const QString &schemaName, const QString &tableName, int dmlType /*OraExp::DmlType*/);

    void addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title);
    void handleDynamicAction();

    void openFile();

    DbItemAction *getSenderAction() const;

    void closeTab(QWidget *widget);

    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className = QString(), int limit = -1);
private:
    DbConnection *db;
    ConnectionPage *cnPage;

    QString getCorrectSchemaNameForCurrentContext();
    QString createTabId(const QString &prefix, const QString &objectTypeName, const QString &schemaName, const QString &objectName) const;
};

#endif // DBUIMANAGER_H
