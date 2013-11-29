#ifndef DBUIMANAGER_H
#define DBUIMANAGER_H

#include <QObject>
#include "../navtree/dbtreemodel.h"
#include "db_object_creator/dbobjectcreator.h"
#include "enums.h"

class DbConnection;
class ConnectionPage;
class DbItemAction;
class ConnectionPageObject;
class ConnectionPageTab;
class Worksheet;
class ObjectLookupDialog;

class DbUiManager : public QObject
{
    Q_OBJECT
public:
    explicit DbUiManager(QObject *parent = 0);
    virtual ~DbUiManager();

signals:

public slots:
    void setConnection(DbConnection *db);

    void refreshTreeNodeChildren();

    void createCreator();
    void createCreator(DbItemAction *action);
    void createCreator(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType);
    void createCreator(const QString &schemaName, const QString &objectName, const QString &itemTypeName);
    void createEditor(DbObjectCreator::CreatorMode creatorMode=DbObjectCreator::EditExisting);
    void createEditor(const QString &schemaName,
                      const QString &objectName,
                      const DbTreeModel::DbTreeNodeType itemType,
                      DbObjectCreator::CreatorMode creatorMode=DbObjectCreator::EditExisting,
                      QHash<QString, QString> properties = QHash<QString, QString>());
    void createViewer();
    void createViewer(const QString &schemaName, const QString &objectName, int itemType /*DbTreeModel::DbTreeNodeType*/);
    //void createViewer(const QString &schemaName, const QString &objectName, const QString &itemTypeName);

    void createEditor(const QString &schemaName, const QString &objectName, int itemType /*DbTreeModel::DbTreeNodeType*/,
                      const QString &tabName = QString(), const QString &childObjectName = QString());
    void createEditor(const QString &schemaName, const QString &objectName, const QString &objectTypeName,
                      const QString &tabName = QString(), const QString &childObjectName = QString());
    void createLikeEditor(const QString &schemaName, const QString &objectName, int itemType /*DbTreeModel::DbTreeNodeType*/);

    QString getViewerTitle(DbTreeModel::DbTreeNodeType objectType, const QString &schemaName, const QString &objectName) const;

    Worksheet *addWorksheet(const QString &contents="");
    Worksheet *getWorksheet();
    void addSchemaComparer();
    void addSchemaExporter();
    void addDataComparer(const QString &schemaName = QString(), const QString &objectName = QString());
    void addDataCopier(const QString &schemaName = QString(), const QString &objectName = QString());
    void addDataExporter(const QString &schemaName = QString(), const QString &objectName = QString());
    void setProperties(ConnectionPageTab *tab, const QString &schemaName, const QString &objectName);
    void setProperties(ConnectionPageTab *tab);
    void addDataImporter();
    void addDataImporter(const QString &schemaName, const QString &tableName);

    void addDmlGenerator(const QString &schemaName, const QString &tableName, int dmlType /*OraExp::DmlType*/);
    void addDdlGenerator(const QString &schemaName, const QString &tableName);

    void showObjectLookupDialog();

    void addSessionBrowser();

    void describeObject(const QString &objectName);

    void showRecordCount(const QString &query, const QList<Param*> &params);

    void addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title);
    void handleDynamicAction();

    void openFiles();
    void openFile(const QString &fileName);
    void openRecentFile(const QString &filename);

    ConnectionPage *getConnectionPage() const {return this->cnPage;}

    DbItemAction *getSenderAction() const;

    void closeTab(QWidget *widget);

    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className = QString(), int limit = -1);
private:
    DbConnection *db;
    ConnectionPage *cnPage;

    ObjectLookupDialog *objectLookupDialog;

    QString getCorrectSchemaNameForCurrentContext();
    QString createTabId(const QString &prefix, const QString &objectTypeName, const QString &schemaName, const QString &objectName) const;
};

#endif // DBUIMANAGER_H
