#ifndef DBUIMANAGER_H
#define DBUIMANAGER_H

#include <QObject>
#include "../navtree/dbtreemodel.h"

class DbConnection;
class ConnectionPage;
class DbItemAction;
class ConnectionPageTab;

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

    void addWorksheet(const QString &contents="");
    void addSchemaComparer();
    void addSchemaExporter();
    void addDataComparer();
    void addDataCopier();
    void addDataExporter();

    DbItemAction *getSenderAction() const;

    void closeTab(QWidget *widget);

    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className = QString(), int limit = -1);
private:
    DbConnection *db;
    ConnectionPage *cnPage;

    QString getCorrectSchemaNameForCurrentContext();
};

#endif // DBUIMANAGER_H
